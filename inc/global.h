#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXRESERVED 12
#define MAXTOKENLEN 40

// token

typedef enum {
  ENDFILE, MAIN, 
  IF, ELSE, SWITCH, CASE, WHILE,
  INT, BOOLEAN, CHAR, STRING,
  TRUE, FALSE,
  ID, NUM, 
  EQ, NE, LT, LE, GT, GE, NOT,
  PLUS, MINUS, TIMES, OVER, MODULO, 
  ASSIGN, SEMI, LPAREN, RPAREN, LBRACE, RBRACE, COLON, OTHER, 
  COMMENT 
} TokenType;

// compiler I/O

extern FILE *source;  // source file
extern FILE *code;    // output asm file
extern int line_no;   // source file line number

// syntax tree

typedef enum {STMTK, EXPK} NodeKind;
typedef enum {IFK, REPEATK, ASSIGNK, READK, WRITEK} StmtKind;
typedef enum {OPK, CONSTK, IDK} ExpKind;
typedef enum {VOID, INTEGER, BOOL} ExpType;

#define CHILDREN 3

typedef struct treeNode {
  struct treeNode *children[CHILDREN];
  struct treeNode *sibling;
  int line;
  NodeKind nkind;
  union {StmtKind stmt; ExpKind exp;} kind;
  union {TokenType op; int val; char *name;} attr;
  ExpType type;
} TreeNode;

#endif
