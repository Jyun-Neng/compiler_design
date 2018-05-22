#include "global.h"
#include "parser.h"
#include "scanner.h"

static TokenType current_token;

static void syntaxError(char *errmsg);
static void match(TokenType expected);
static void main_stmt();
static void body();

static void syntaxError(char *errmsg) {
  fprintf(stdout, "\n>>> ");
  fprintf(stdout, "Syntax error at line %d: %s", line_no, errmsg);
}

static void match(TokenType expected) {
  if (current_token == expected) current_token = getToken();
  else {
    syntaxError("unexpected token -> ");
    fprintf(stdout, "%s\n", token);
    exit(1);
  }
}

void parser() {
  current_token = getToken();

  if (current_token != ENDFILE) {
    while (current_token == COMMENT) current_token = getToken();
  }

  main_stmt();
}

static void main_stmt() {
  if (current_token == MAIN) {
    current_token = getToken();
    match(LPAREN);
    match(RPAREN); 
    while (current_token == '\t' || current_token == '\n') {
      if ((current_token = getToken()) == LBRACE) break;
    }
    match(LBRACE);
    body();
  }
  else syntaxError("no main function"); 
}

static void body() {
  while (current_token != RBRACE) {
      current_token = getToken();
      if (current_token == ENDFILE) break;
  }
  match(RBRACE);
}
