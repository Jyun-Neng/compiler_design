#ifndef _SCANNER_H_
#define _SCANNER_H_
#define MAXTOKENLEN 40
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char token[MAXTOKENLEN];
FILE *fptr;

typedef enum {
  ENDFILE,  
  IF, ELSE, SWITCH, CASE, WHILE,
  INT, BOOLEAN, CHAR, STRING,
  TRUE, FALSE,
  ID, NUM, 
  EQ, NE, LT, LE, GT, GE, NOT,
  PLUS, MINUS, TIMES, OVER, MODULO, 
  ASSIGN, SEMI, LPAREN, RPAREN, COLON, OTHER, 
  COMMENT 
} TokenType;

TokenType getToken();

#endif
