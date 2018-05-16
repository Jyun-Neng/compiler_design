#include <stdio.h>
#include "scanner.h"

int main(int argc, const char *argv[])
{
    char c;
    TokenType t;
    extern FILE *fptr;
    extern char token[MAXTOKENLEN];
    fptr = fopen(argv[1], "r");
    while ((c=getc(fptr)) != EOF) {
      ungetc(c, fptr);
      t = getToken();
      printf("%s\n",token);
      printf("token type %u\n", t);
    }
    return 0;
}
