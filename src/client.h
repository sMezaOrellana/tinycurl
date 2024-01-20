#ifndef CLIENT_H
#define CLIENT_H
#include "cli.h"
#include "http.h"
#include "procotolindependent.h"
#include "uri.h"
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_RESPONSE (1024 * 6)
#define MAX_RETRIES 5

typedef struct _requestwrapper {
  clifields *cli;
  int index;
  genericheaders *headers;
  uri *uri;
} requestwrapper;

typedef struct _requests {
  requestwrapper *requestwrappers;
  int numrequests;
} requests;

void *makerequestwrapper(void *requeststruct);
void makerequest(clifields *cli, int index, genericheaders *headers, uri *uri);

struct addrinfo *getaddrinfofromhost(const char *hostname, const char *port);
int startclient(struct addrinfo *peeraddress);
void freeclient(int fd);
int readsocket(int fd, char *buffer, int size);
int writesocket(int fd, char *msg, int size);
int configureclient(uri *uri);
#endif
