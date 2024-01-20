#include "http.h"

#include "procotolindependent.h"

const char *REQUESTLINE = "%s %s %s\r\n";
const char *HEADER = "%s: %s\r\n";
const char *LINEENDING = "%s\r\n";
const char *ENTITYBODY = "%s%s";

void inithttprequest(httprequest *request, int size) {
  request->buffer = (char *)malloc(size);
  request->size = size;
}

int addhttprequestline(char *buffer, const char *method, const char *requesturi,
                       const char *httpversion) {
  return (int)sprintf(buffer, REQUESTLINE, method, requesturi, httpversion);
}

int addhttpheader(char *buffer, const char *field, const char *value) {
  return (int)sprintf(buffer, HEADER, field, value);
}

int addhttplineending(char *buffer) {
  // use memcopy as src and des overlap
  return (int)sprintf(buffer, LINEENDING, buffer);
}

void addhttpheaders(httprequest *request, genericheaders *headers,
                    const char *hostname) {

  for (int i = 0; i < headers->available; i++) {
    int bytes = snprintf(request->buffer + request->pos, MAXHEADERSIZE,
                         "%s\r\n", headers->data[i]);
    request->pos += bytes;
  }
  request->pos +=
      addhttpheader(request->buffer + request->pos, "Accept", "*/*");
  request->pos +=
      addhttpheader(request->buffer + request->pos, "Host", hostname);
  request->pos +=
      addhttpheader(request->buffer + request->pos, "Connection", "close");
  request->pos += addhttplineending(request->buffer + request->pos);
}

httprequest *configurerequest(uri *uri, clifields *cli,
                              genericheaders *headers) {

  httprequest *req = malloc(sizeof(httprequest));
  inithttprequest(req, HTTPREQUESTSIZE);

  if ((strcmp(uri->scheme, "") == 0) || (strcmp(uri->scheme, "http") == 0)) {
    /* handle http request */
    char *head = req->buffer;
    req->pos += addhttprequestline(head, "GET", uri->path, "HTTP/1.1");
    addhttpheaders(req, headers, uri->host);

  } else {
    fprintf(stderr, "Schema: (%s) is not supported\n", uri->scheme);
    exit(1);
  }

  return req;
}
