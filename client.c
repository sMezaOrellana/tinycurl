#include "client.h"
extern pthread_mutex_t stdout_mutex;
void *makerequestwrapper(void *requeststruct) {
  requestwrapper *rwrap = (requestwrapper *)requeststruct;
  makerequest(rwrap->cli, rwrap->index, rwrap->headers, rwrap->uri);
  pthread_exit(NULL);
}

void makerequest(clifields *cli, int index, genericheaders *headers, uri *uri) {
  clock_t start = clock();
  char *uristr = cli->hostnames[index];

  if (!parseuri(uri, (const char **)&uristr)) {
    fprintf(stderr, "uri was not parsed correctly\n");
    exit(1);
  }

  int socketfd = configureclient(uri);
  httprequest *req = configurerequest(uri, cli, headers);

  writesocket(socketfd, req->buffer, req->size);
  char resp[MAX_RESPONSE];
  readsocket(socketfd, resp, MAX_RESPONSE);

  clock_t end = clock();

  pthread_mutex_lock(&stdout_mutex);
  printf("%s", req->buffer);
  printf("HTTP request for: (%s)\ntook: (%f) seconds\n", cli->hostnames[index],
         ((double)(end - start)) / CLOCKS_PER_SEC);
  pthread_mutex_unlock(&stdout_mutex);
  free(req->buffer);
  free(req);
  close(socketfd);
}

struct addrinfo *getaddrinfofromhost(const char *hostname, const char *port) {
  struct addrinfo hints = {0};
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;
  struct addrinfo *peer_address;
  if (getaddrinfo(hostname, port, &hints, &peer_address)) {
    perror("getaddrinfo()");
    exit(0);
  }

  printf("Remote address is: ");
  char address_buffer[100];
  char service_buffer[100];

  getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer,
              sizeof(address_buffer), service_buffer, sizeof(service_buffer),
              NI_NUMERICHOST);
  printf("%s %s\n", address_buffer, service_buffer);

  return peer_address;
}

int startclient(struct addrinfo *peeraddress) {
  int socketfd = socket(peeraddress->ai_family, peeraddress->ai_socktype,
                        peeraddress->ai_protocol);
  if (socketfd < 0) {
    perror("socket()");
    exit(1);
  }

  if (connect(socketfd, peeraddress->ai_addr, peeraddress->ai_addrlen)) {
    perror("connect()");
    exit(1);
  }
  freeaddrinfo(peeraddress);
  return socketfd;
}

int writesocket(int fd, char *msg, int size) {
  int bytes_sent = 0;
  int retries = 0;
  int total_sent = 0;

  while (retries < MAX_RETRIES && size > 0) {
    bytes_sent = write(fd, msg, size);

    if (bytes_sent > 0) {
      msg += bytes_sent;
      size -= bytes_sent;
      total_sent += bytes_sent;
    } else {
      retries++;
    }
  }

  if (bytes_sent >= 0) {
    // Last write was not an error, return how many bytes were sent
    return total_sent;
  }
  // Last write was an error, return error code
  return -1;
}

int readsocket(int fd, char *buffer, int size) {
  int bytes_recvd = 0;
  int retries = 0;
  int total_recvd = 0;

  char *bufferptr = buffer;

  while (retries < MAX_RETRIES && size > 0) {
    bytes_recvd = read(fd, bufferptr, size);

    if (bytes_recvd > 0) {
      bufferptr += bytes_recvd;
      size -= bytes_recvd;
      total_recvd += bytes_recvd;
    } else {
      retries++;
    }
  }
  if (bytes_recvd >= 0) {
    // Last read was not an error, return how many bytes were recvd
    printf("%s\n", buffer);
    return total_recvd;
  }
  // Last read was an error, return error code
  return -1;
}

int configureclient(uri *uri) {
  char *port = strcmp(uri->port, "") == 0 ? "80" : uri->port;
  struct addrinfo *peeraddress = getaddrinfofromhost(uri->host, port);
  int socketfd = startclient(peeraddress);
  return socketfd;
}
