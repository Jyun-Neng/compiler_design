#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *source;
FILE *code;
char *codefile;
int line_no = 1;

int main(int argc, const char *argv[]) {
  int len;

  if (argc != 2) {
    fprintf(stderr, "No source file.\n");
    exit(1);
  }

  // create asm file
  len = strcspn(argv[1], ".");
  codefile = (char *)malloc((len + 4) * sizeof(char));
  strncpy(codefile, argv[1], len);  // strncpy does not copy null character
  codefile[len] = '\0';
  strcat(codefile, ".asm");
  code = fopen(codefile, "w");

  // read source file
  source = fopen(argv[1], "r");
  if (source == NULL) {
    fprintf(stderr, "File %s not found\n", argv[1]);
    exit(1);
  }
  fprintf(stdout, "\n Compile start, please wait.... \n");

  parser();

  fprintf(stdout, "\n Parsing end. Line %d\n", line_no);

  return 0;
}
