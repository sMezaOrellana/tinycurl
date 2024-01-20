#ifndef HTTP_H
#define HTTP_H

#include "cli.h"
#include "procotolindependent.h"
#include "uri.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HTTPREQUESTSIZE 4096

typedef struct httprequest_ {
  char *buffer;
  int pos;
  int size;
} httprequest;

void buildhttprequest(httprequest *request, const char *method,
                      genericheaders *headers, const char *data);
void buildhttpheaders(httprequest *request, char *headers);
int addhttprequestline(char *buffer, const char *method, const char *requesturi,
                       const char *httpversion);
int addhttpheader(char *buffer, const char *field, const char *value);
void addhttpheaders(httprequest *request, genericheaders *headers,
                    const char *hostname);
int addhttplineending(char *buffer);
void inithttprequest(httprequest *request, int size);
httprequest *configurerequest(uri *uri, clifields *cli,
                              genericheaders *headers);
#endif
