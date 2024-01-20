#include "cli.h"
#include "client.h"
#include "http.h"
#include "uri.h"

#include <time.h>

pthread_mutex_t stdout_mutex = PTHREAD_MUTEX_INITIALIZER;

void printusage(const char *program_name) {
  printf("Usage: %s [options]\n", program_name);
  printf("Options:\n");
  printf("  -p <port>       Specify the port to connect to\n");
  printf("  -X <method>     Specify the HTTP method to use\n");
  printf(
      "  -H <header>     Specify an HTTP header to include in the request\n");
  printf("  -u <uri>        Specify the URI to request\n");
  printf("  -help           Display this help message\n");
}

int main(int argc, char *argv[]) {

  if (argc == 1) {
    printusage(argv[0]);
    exit(1);
  }

  clifields cli = {0};
  genericheaders headers = {0};
  uri uri = {0};

  initheaders(&headers);
  parsecliarguments(&cli, argc, argv, &headers);

  pthread_t threads[cli.numhostnames];
  clock_t start = clock();
  for (int i = 0; i < cli.numhostnames; i++) {
    requestwrapper reqwrap = {&cli, i, &headers, &uri};
    pthread_create(&threads[i], NULL, makerequestwrapper, (void *)&reqwrap);
  }

  for (int i = 0; i < cli.numhostnames; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      fprintf(stderr, "Error joining thread: %d\n", i);
      exit(1);
    }
  }

  clock_t end = clock();
  printf("HTTP requests took: (%f) seconds\n",
         ((double)(end - start)) / CLOCKS_PER_SEC);

  freeclifields(&cli);
  freeheaders(&headers);
  freeuri(&uri);
  return 0;
}
