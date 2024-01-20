#ifndef CLI_H
#define CLI_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "http.h"
#include "procotolindependent.h"
#define MAXHOSTNAMES (24)
typedef struct cli_ {
  int numhostnames;
  char *hostname;
  char **hostnames;
  bool hostnameflag;

  char *port;
  bool portflag;

  char *method;
  bool methodflag;

  char *uri;
  bool uriflag;

  bool headersflag;
} clifields;

void parsecliarguments(clifields *cli, int argc, char **argv,
                       genericheaders *headers);
void freeclifields(clifields *cli);
#endif
