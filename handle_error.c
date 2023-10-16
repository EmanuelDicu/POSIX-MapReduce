#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void
handle_error_en(int en, char *msg) 
{
    errno = en;
    fprintf(stderr, "%s\n", msg);
    exit(-1);
}

void 
handle_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(-1);
}