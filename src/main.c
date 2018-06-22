#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

FILE *source;
FILE *code;
char *sourcefile;
char *codefile;
int line_no = 1;
int ERROR = 0;
int dcl_n = 0;
const char *token_type[] = {
    "ENDFILE", "main",     "if",    "else",    "switch", "case",   "while",
    "int",     "bool",     "char",  "string",  "TRUE",   "FALSE",  "ID",
    "NUM",     "==",       "!=",    "<",       "<=",     ">",      ">=",
    "NOT",     "+",        "-",     "*",       "/",      "%",      "=",
    ";",       ",",        "(",     ")",       "{",      "}",      ":",
    "STRTYPE", "CHARTYPE", "OTHER", "COMMENT", "print",  "println"};

int main(int argc, const char *argv[]) {
  int len;
  int flen;

  if (argc != 2) {
    fprintf(stderr, "No source file.\n");
    exit(1);
  }
  // store source file name
  flen = strlen(argv[1]);
  sourcefile = (char *)malloc((flen + 2) * sizeof(char));
  strcat(sourcefile, "./");
  strcat(sourcefile, argv[1]);

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

  if (ERROR > 1) fprintf(stderr, " %d errors generated.\n", ERROR);
  if (ERROR == 1) fprintf(stderr, " %d error generated.\n", ERROR);
  if (ERROR != 0) exit(1);

  fprintf(stdout, "\n Compiled succeed!\n");

  return 0;
}
