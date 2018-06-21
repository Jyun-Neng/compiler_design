#include "parser.h"
#include "cgen.h"
#include "global.h"
#include "scanner.h"
#include "symtab.h"

char *dcl_name;
int dcl_val;
TokenType dcl_type;

static TokenType current_token;

static void syntaxError(int line, char *errmsg);
static void match(int line, TokenType expected);
static void main_stmt();
static void body();
static void const_dcl();
static void var_list();
/* The data type of constant variable is defined by the assigned data type. */
static void const_stmt();
static void var_stmt(TokenType dcl_type);
static void assign_stmt();
static void print_stmt();
static void println_stmt();
static void if_stmt();
static void exp();
static void simple_exp();
static void term();
static void factor();

static void syntaxError(int line, char *errmsg) {
  fprintf(stderr, "%s:%d:%d: syntax error: %s", sourcefile, line_no, line,
          errmsg);
  ERROR++;
}

static void match(int line, TokenType expected) {
  if (current_token == expected) {
    current_token = getToken();
  } else {
    syntaxError(line, "unexpected token ");
    fprintf(stderr, "\"%s\"\n", token);
    current_token = getToken();
  }
}

void parser() {
  st_init();
  current_token = getToken();

  if (current_token != ENDFILE) {
    while (current_token == COMMENT) current_token = getToken();
  }

  cgen('H');
  main_stmt();
  cgen('E');
}

static void main_stmt() {
  if (current_token == MAIN) {
    current_token = getToken();
    match(__LINE__, LPAREN);
    match(__LINE__, RPAREN);
    body();
  } else
    syntaxError(__LINE__, "no main function");
}

static void body() {
  match(__LINE__, LBRACE);
  while (current_token != RBRACE) {
    switch (current_token) {
      case CONST:     // declaration statement
        const_dcl();
        break;
      case STRING:    // declaration statement
      case INT:       // declaration statement
      case CHAR:      // declaration statement
      case BOOLEAN:   // declaration statement
        var_list();
        break;
      case ID:
        op_init();
        assign_stmt();
        break;
      case PRINT:
        op_init();
        print_stmt();
        break;
      case PRINTLN:
        op_init();
        println_stmt();
        break;
      case IF:
        if_stmt();
        break;
      case COMMENT:
        match(__LINE__, COMMENT);
        break;
      default:
        syntaxError(__LINE__, "unexpected token ");
        fprintf(stderr, "\"%s\"\n", token);
        match(__LINE__, current_token);
        break;
    }
  }
  match(__LINE__, RBRACE);
}

static void const_dcl() {
  match(__LINE__, CONST);
  const_stmt();
  while ((current_token = getToken()) == COMMA) {
    current_token = getToken();
    const_stmt();
  }
  match(__LINE__, SEMI);
}

static void var_list() {
  TokenType dcl_type = current_token;
  switch (current_token) {
    case STRING:
    case CHAR:
    case INT:
    case BOOLEAN:
      var_stmt(dcl_type);
      break;
    default:
      break;
  }
  while (current_token == COMMA) {
    var_stmt(dcl_type);
  }

  match(__LINE__, SEMI);
}

static void const_stmt() {
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  strcat(dcl_name, token);  // store declaration variable name
  match(__LINE__, ID);
  match(__LINE__, ASSIGN);
  switch (current_token) {
    case ID:
      st_insert(dcl_name, 0, token, current_token);
      break;
    case NUM:
      st_insert(dcl_name, 0, token, current_token);
      break;
    case TRUE:
      st_insert(dcl_name, 0, token, current_token);
      break;
    case FALSE:
      st_insert(dcl_name, 0, token, current_token);
      break;
    case STRTYPE:
      st_insert(dcl_name, 0, token, current_token);
      break;
    case CHARTYPE:
      st_insert(dcl_name, 0, token, current_token);
      break;
    default:
      syntaxError(__LINE__, "unexpected token ");
      fprintf(stderr, "\"%s\"\n", token);
      exit(1);
      break;
  }
}

static void var_stmt(TokenType dcl_type) {
  current_token = getToken();
  char *dcl_name = malloc(sizeof(char));
  char *dcl_val = malloc(sizeof(char));
  strcpy(dcl_name, token);
  if ((current_token = getToken()) == ASSIGN) {
    current_token = getToken();
    switch (dcl_type) {
      case STRING:
        if (current_token != STRTYPE && current_token != ID) {
          printf("var assign error\n");
          exit(1);
        }
        dcl_type = current_token;
        break;
      case CHAR:
        if (current_token != CHARTYPE && current_token != ID) {
          printf("var assign error\n");
          exit(1);
        }
        dcl_type = current_token;
        break;
      case INT:
        if (current_token != NUM && current_token != ID) {
          printf("var assign error\n");
          exit(1);
        }
        dcl_type = current_token;
        break;
      case BOOLEAN:
        if (current_token != TRUE && current_token != FALSE &&
            current_token != ID) {
          printf("var assign error\n");
          exit(1);
        }
        dcl_type = current_token;
        break;
      default:
        break;
    }
    st_insert(dcl_name, 0, token, dcl_type);
    current_token = getToken();
  } else {
    switch (dcl_type) {
      case STRING:
        dcl_type = STRTYPE;
        dcl_val = " ";
        break;
      case CHAR:
        dcl_type = CHARTYPE;
        dcl_val = " ";
        break;
      case INT:
        dcl_type = NUM;
        dcl_val = "0";
        break;
      case BOOLEAN:
        dcl_type = BOOLEAN;
        dcl_val = "0";
        break;
      default:
        break;
    }
    st_insert(dcl_name, 0, dcl_val, dcl_type);
  }
}

static void assign_stmt() {
  push_operand(st_lookup(token, 0));
  match(__LINE__, ID);
  push_operator(current_token);
  match(__LINE__, ASSIGN);
  exp();
  match(__LINE__, SEMI);
  codegen(pop_operator(), pop_operand(), pop_operand());
}

static void print_stmt() {
  match(__LINE__, PRINT);
  match(__LINE__, LPAREN);
  exp();
  match(__LINE__, RPAREN);
  match(__LINE__, SEMI);
  push_operator(PRINT);
  codegen(pop_operator(), pop_operand(), NULL);
}

static void println_stmt() {
  match(__LINE__, PRINTLN);
  match(__LINE__, LPAREN);
  exp();
  match(__LINE__, RPAREN);
  match(__LINE__, SEMI);
  push_operator(PRINTLN);
  codegen(pop_operator(), pop_operand(), NULL);
}

static void if_stmt() {
  match(__LINE__, IF);
  match(__LINE__, LPAREN);
  exp();
  match(__LINE__, RPAREN);
}

static void exp() {
  simple_exp();
  if (current_token == LT || current_token == NE || current_token == EQ ||
      current_token == LE || current_token == GT || current_token == GE) {
    push_operator(current_token);
    match(__LINE__, current_token);
    simple_exp();
    codegen(pop_operator(), pop_operand(), pop_operand());
  }
}

static void simple_exp() {
  term();
  while (current_token == PLUS || current_token == MINUS) {
    push_operator(current_token);
    match(__LINE__, current_token);
    term();
    codegen(pop_operator(), pop_operand(), pop_operand());
  }
}

static void term() {
  factor();
  while (current_token == TIMES || current_token == OVER ||
         current_token == MODULO) {
    push_operator(current_token);
    match(__LINE__, current_token);
    factor();
    codegen(pop_operator(), pop_operand(), pop_operand());
  }
}

static void factor() {
  switch (current_token) {
    case LPAREN:
      match(__LINE__, LPAREN);
      exp();
      match(__LINE__, RPAREN);
      break;
    case ID:
      push_operand(st_lookup(token, 0));
      match(__LINE__, current_token);
      break;
    case NUM:
    case TRUE:
    case FALSE:
    case STRTYPE:
    case CHARTYPE:
      dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
      dcl_name = "_S";
      st_insert(dcl_name, ++dcl_n, token, current_token);
      push_operand(st_lookup(dcl_name, dcl_n));
      match(__LINE__, current_token);
      break;
    default:
      fprintf(stdout, "factor error\n");
      current_token = getToken();
      break;
  }
}
