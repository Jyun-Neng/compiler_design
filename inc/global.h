#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>

#define MAXRESERVED 15
#define MAXTOKENLEN 40

// token

typedef enum {
  ENDFILE,
  MAIN,
  IF,
  ELSE,
  SWITCH,
  CASE,
  WHILE,
  INT,
  BOOLEAN,
  CHAR,
  STRING,
  CONST,
  TRUE,
  FALSE,
  ID,
  NUM,
  EQ,
  NE,
  LT,
  LE,
  GT,
  GE,
  NOT,
  PLUS,
  MINUS,
  TIMES,
  OVER,
  MODULO,
  ASSIGN,
  SEMI,
  COMMA,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  COLON,
  STRTYPE,
  CHARTYPE,
  OTHER,
  COMMENT,
  PRINT,
  PRINTLN
} TokenType;

// compiler I/O

extern FILE *source;     // source file
extern FILE *code;       // output asm file
extern char *codefile;   // asm file name
extern char *sourcefile; // source file name
extern int line_no;      // source file line number
extern int ERROR;

#endif
