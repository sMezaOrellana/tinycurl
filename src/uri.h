#ifndef URI_H
#define URI_H

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPROTOCOLSIZE (512)
#define MAXHOSTNAMESIZE (1048)
#define MAXSUBDOMAINSIZE (256)
#define MAXWORD (64)
typedef struct uri_ {
  char *scheme;
  char *userinfo;
  char *host;
  char *port;
  char *path;
  char *query;
  char *fragment;
} uri;

typedef int (*functionptr)(int);
bool parseuri(uri *uri, const char **uristr);
void freeuri(uri *uri);

#endif
