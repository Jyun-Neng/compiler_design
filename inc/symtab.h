#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "global.h"

// declaration variable struct node
typedef struct symbol {
  char dcl_name[MAXTOKENLEN];
  int n;
  union dcl_val {
    int num;
    char str[MAXTOKENLEN];
  } dcl_val;
  TokenType dcl_type;
  struct symbol *next;
} Symbol;

// expression operand struct node
typedef struct Operand {
  char op_name[MAXTOKENLEN];
  int n;
  TokenType op_type;
  struct Operand *next;
} Operand;

// expression operator struct node
typedef struct Operator {
  TokenType opr_type;
  struct Operator *next;
} Operator;

/* Symbol table initialization. */
void st_init();
/* Operand and operator stack initialization. */
void op_init();
/* There are 4 declaration data type, STRTYPE, CHARTYPE, NUM, and BOOLEAN.
 * Constant declaration data type is according to its assign value. If assigned
 * data is TRUE or FALSE, constant declaration data type will be BOOLEAN. If
 * assigned data is another declaration data, using st_lookup function to find
 * the data and get its data type.
 */
void st_insert(char *dcl_name, int n, char *dcl_char,
               TokenType dcl_type);
/* Pop symbol structure node from symbol table. */
Symbol *st_pop();
/* Search if the identifier is declared. */
Symbol *st_lookup(char *srch_name, int n);
/* Print symbol table. */
void st_print();
/* Add operand symbol to operand stack. */
void push_operand(Symbol *op);
/* Add operator type to operator stack. */
void push_operator(TokenType opr_type);
/* Pop operand out from the operand stack. */
Operand *pop_operand();
/* Pop operator out from the operator stack. */
Operator *pop_operator();

#endif
