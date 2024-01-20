#include "procotolindependent.h"

void initheaders(genericheaders* headers)
{
    headers->size = MAXHEADERS;
    headers->data = (char **) malloc(MAXHEADERS * sizeof(char*));
    headers->available = 0;
}

bool addheader(genericheaders* headers, const char* header)
{
    if(headers->available >= headers->size)
    {
        //add logging statement
        return false;
    }

    headers->data[headers->available] = strdup(header);
    headers->available += 1;
    return true;
}

bool freeheaders(genericheaders* headers)
{
    
    if(headers->available >= headers->size)
    {
        //add logging statement
        return false;
    }

    for(int i=0; i < headers->available; i++)
    {
        free(headers->data[i]);
    }

    free(headers->data);
    return true;
}
