#include "cli.h"

void parsecliarguments(clifields *cli, int argc, char **argv,
                       genericheaders *headers) {

  /*TODO: function needs a rewrite to better handle the parsing of commandline
   * arguments */
  cli->numhostnames = 0;
  cli->hostnames = (char **)malloc(MAXHOSTNAMES * sizeof(char *));

  int option;
  while ((option = getopt(argc, argv, "H:p:h:u:X:")) != -1) {
    switch (option) {
    case 'p': {
      cli->port = strdup(optarg);
      cli->portflag = true;
      break;
    }
    case 'h': {
      printf("Hostname: %s\n", optarg);
      cli->hostname = strdup(optarg);
      cli->hostnameflag = true;
      break;
    }
    case 'X': {
      if (cli->uriflag == true)
        break;
      printf("HTTP Method: %s\n", optarg);
      cli->method = strdup(optarg);
      cli->methodflag = true;
      break;
    }
    case 'H': {
      printf("Header: %s\n", optarg);
      addheader(headers, optarg);
      cli->headersflag = true;
      break;
    }
    case 'u': {
      if (cli->uriflag == true)
        break;
      cli->uri = strdup(optarg);
      cli->uriflag = true;
      break;
    }
    default:
      break;
    }
  }

  for (int i = optind; i < argc; i++) {
    cli->hostnames[cli->numhostnames] = strdup(argv[i]);
    cli->numhostnames += 1;
  }
}

void freeclifields(clifields *cli) {
  if (cli->portflag)
    free(cli->port);

  for (int i = 0; i < cli->numhostnames; i++) {
    free(cli->hostnames[i]);
  }

  free(cli->hostnames);

  if (cli->hostnameflag)
    free(cli->hostname);

  if (cli->methodflag)
    free(cli->method);

  if (cli->uriflag)
    free(cli->uri);
}
