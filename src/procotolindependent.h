#ifndef PROTIND_H
#define PROTIND_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAXHEADERS 255
#define MAXHEADERSIZE 255
typedef struct genericheaders_
{
  char** data;
  int size;
  int available;
} genericheaders;

bool freeheaders(genericheaders* headers);
void initheaders(genericheaders* headers);

bool addheader(genericheaders* headers, const char* header);
#endif