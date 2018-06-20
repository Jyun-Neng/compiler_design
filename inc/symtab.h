#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "global.h"

typedef struct Operand {
  char *op_name;
  int n;
  TokenType op_type;
  struct Operand *next;
} Operand;

typedef struct Operator {
  TokenType opr_type;
  struct Operator *next;
} Operator;

Operand *operand;
Operator *operator;

typedef struct symbol {
  char *dcl_name;
  int n;
  union dcl_val {
    int num;
    char *str;
  } dcl_val;
  TokenType dcl_type;
  struct symbol *next;
} Symbol;

Symbol *sym;

void st_init();  // symbol table initialization
void op_init();
void operand_traversal();
/*
 * There are 4 declaration data type, STRTYPE, CHARTYPE, NUM, and BOOLEAN.
 * Constant declaration data type is according to its assign value. If assigned
 * data is TRUE or FALSE, constant declaration data type will be BOOLEAN. If
 * assigned data is another declaration data, using st_lookup function to find
 * the data and get its data type.
 */
void st_insert(char *dcl_name, char *dcl_char,
               TokenType dcl_type);  // add new data
Symbol *st_lookup(char *srch_name);  // find data in the symbol table
void st_print();  // print data that store in the symbol table
void push_operand(Symbol *op);
void push_operator(TokenType opr_type);
Operand *pop_operand();
Operator *pop_operator();

#endif
