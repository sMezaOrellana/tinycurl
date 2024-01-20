#include "uri.h"

bool word(const char **uristr, char **alphaword, functionptr func);
bool parseschema(uri *uri, const char **uristr);
bool parsehost(uri *uri, const char **uristr);
bool parseport(uri *uri, const char **uristr);
bool parsepath(uri *uri, const char **uristr);

/*
  URI -> SCHEME? HOST PATH QUERY? FRAG?
  SCHEME -> ALPHAWORD ':'
  HOST -> '//' (ALPHAWORD '.')+ ALPHAWORD
  ALPHAWORD -> ([a-z]/[A-Z])
*/
bool parseuri(uri *uri, const char **uristr) {
  /*
    TODO:
    ensure on match and failure to initialize all members.
    so outside of this function we know how to free uri.
  */
  char *startpos = (char *)*uristr;

  if (!parseschema(uri, uristr)) {
    free(uri->scheme);
    uri->scheme = strdup("");
    *uristr = startpos;
    goto host;
  }

  /*the first '/' must be there */
  assert(**uristr == '/');
  (*uristr)++;

  /*the second '/' is optional */
  assert(**uristr == '/');
  (*uristr)++;

host:
  if (!parsehost(uri, uristr)) {
    return false;
  }

  startpos = (char *)*uristr;
  if (!parseport(uri, uristr)) {
    free(uri->port);
    uri->port = strdup("");
    *uristr = startpos;
    goto path;
  }

path:
  if (!parsepath(uri, uristr)) {
    if (strcmp(uri->path, "") == 0) {
      free(uri->path);
      uri->path = strdup("/");

    } else {
      return false;
    }
  }
  assert(**uristr == '\0');
  return true;
}

bool parseport(uri *uri, const char **uristr) {
  if (**uristr != ':') {
    uri->port = strdup("");
    return false;
  }

  (*uristr)++;
  if (!word(uristr, &uri->port, isdigit)) {
    return false;
  }
  return true;
}

bool parsehost(uri *uri, const char **uristr) {
  /*
    add support for ipv4 and ipv6.
    this function does not return false??? need to check this
  */
  char hostbuffer[MAXHOSTNAMESIZE] = {'\0'};
  char *hword;
  while (word(uristr, &hword, isalpha)) {

    if (**uristr != '.') {
      break;
    }

    /*
      this leads to a truncated string and must be further analyzed
      man strncat offers an example on how to deal with that.
      TODO: implement solution from man strncat.
    */
    strncat(hostbuffer, hword, MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
    strncat(hostbuffer, ".", MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
    free(hword);
    (*uristr)++;
  }

  /*
    idem as above usage of strncat
  */
  strncat(hostbuffer, hword, MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
  free(hword);

  uri->host = strdup(hostbuffer);
  return true;
}

bool parsepath(uri *uri, const char **uristr) {

  char hostbuffer[MAXHOSTNAMESIZE] = {'\0'};
  char *hword;

  if (**uristr != '/') {
    uri->path = strdup("");
    return false;
  }

  hostbuffer[0] = '/';
  (*uristr)++;

  while (word(uristr, &hword, isalpha)) {

    if (**uristr != '/') {
      break;
    }

    /*
      this leads to a truncated string and must be further analyzed
      man strncat offers an example on how to deal with that.
      TODO: implement solution from man strncat.
     */
    strncat(hostbuffer, hword, MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
    strncat(hostbuffer, "/", MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
    free(hword);
    (*uristr)++;
  }

  /*
    idem as above usage of strncat
  */
  strncat(hostbuffer, hword, MAXSUBDOMAINSIZE - strlen(hostbuffer) - 1);
  free(hword);

  uri->path = strdup(hostbuffer);
  return true;
}

bool parseschema(uri *uri, const char **uristr) {

  word(uristr, &uri->scheme, isalpha);
  if (strlen(uri->scheme) == 0)
    return false;

  if (**uristr != ':') {
    return false;
  }

  /*
    moving pointer past end of *uristr
    need macro to make this reusable
  */
  (*uristr)++;
  return true;
}

bool word(const char **uristr, char **alphaword, functionptr func) {
  char buffer[MAXWORD] = {'\0'};
  char *pbuf = buffer;
  char *ebuf = buffer + MAXWORD - 1;
  if (!func(**uristr)) {
    *alphaword = strdup("");
    return false;
  }

  while (func(**uristr)) {
    // ensure we do not write after end of buffer
    if (pbuf >= ebuf) {
      /*
        string will be truncated need better way to handle this
      */
      printf("[word()] buffer is truncated: %s\n", buffer);
      *alphaword = strdup(buffer);
      return false;
    }
    *pbuf = **uristr;
    pbuf++;
    (*uristr)++;
  }

  *alphaword = strdup(buffer);
  // printf("alphaword(): alphaword: %s\n", *alphaword);
  return true;
}

void freeuri(uri *uri) {
  free(uri->scheme);
  free(uri->host);
  free(uri->port);
  free(uri->path);
}
