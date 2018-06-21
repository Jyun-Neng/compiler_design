#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol *sym;
static Operand *operand;
static Operator *operator;

void st_init() { sym = NULL; }

void op_init() {
  operand = NULL;
  operator = NULL;
}

void st_insert(char *dcl_name, int n, char *dcl_char, TokenType dcl_type) {
  Symbol *new_sym = (Symbol *)malloc(sizeof(Symbol));
  Symbol *find_sym = (Symbol *)malloc(sizeof(Symbol));
  strcpy(new_sym->dcl_name, dcl_name);
  new_sym->n = n;
  new_sym->dcl_type = dcl_type;
  switch (dcl_type) {
    case STRTYPE:
    case CHARTYPE:
      strcpy(new_sym->dcl_val.str, dcl_char);
      break;
    case NUM:
      new_sym->dcl_val.num = atoi(dcl_char);
      break;
    case TRUE:
      new_sym->dcl_type = BOOLEAN;
      new_sym->dcl_val.num = 1;
      break;
    case FALSE:
      new_sym->dcl_type = BOOLEAN;
      new_sym->dcl_val.num = 0;
      break;
    case BOOLEAN:
      new_sym->dcl_val.num = 0;
      break;
    case ID:
      find_sym = st_lookup(dcl_char, 0);
      new_sym->dcl_val = find_sym->dcl_val;
      new_sym->dcl_type = find_sym->dcl_type;
      break;
    default:
      fprintf(stderr, "%s:%d:%d: warning: Unknown declaration data type.\n",
              sourcefile, line_no, __LINE__);
      ERROR++;
      break;
  }
  new_sym->next = sym;
  sym = new_sym;
}

Symbol *st_pop() {
  Symbol *pop_sym = (Symbol *)malloc(sizeof(Symbol));
  Symbol *tmp = sym;
  if (sym) {
    *pop_sym = *sym;
    sym = sym->next;
    free(tmp);
    return pop_sym;
  } else
    return NULL;
}

Symbol *st_lookup(char *srch_name, int n) {
  Symbol *sym_traverse = sym;
  while (sym_traverse) {
    if ((strcmp(sym_traverse->dcl_name, srch_name) == 0) &&
        (n == sym_traverse->n)) {  // match
      return sym_traverse;
    }
    sym_traverse = sym_traverse->next;
  }
  fprintf(stderr, "%s:%d:%d: error: use of undeclared identifier \"%s\"\n",
          sourcefile, line_no, __LINE__, srch_name);
  ERROR++;
  fprintf(stderr, "%d error generated.\n", ERROR);
  exit(1);
}

void st_print() {
  Symbol *print_sym = sym;
  fprintf(stdout, "Variable Name  Variable Value\n");
  fprintf(stdout, "-------------  --------------\n");
  while (print_sym) {
    fprintf(stdout, "%-14s ", print_sym->dcl_name);
    switch (print_sym->dcl_type) {
      case NUM:
      case BOOLEAN:
        fprintf(stdout, "%-15d\n", print_sym->dcl_val.num);
        break;
      case STRTYPE:
      case CHARTYPE:
        fprintf(stdout, "%s\n", print_sym->dcl_val.str);
        break;
      default:
        fprintf(stdout, "symbol error\n");
        break;
    }
    print_sym = print_sym->next;
  }
}

void push_operand(Symbol *op) {
  Operand *new_op = (Operand *)malloc(sizeof(Operand));
  if (op) {
    strcpy(new_op->op_name, op->dcl_name);
    new_op->n = op->n;
    new_op->op_type = op->dcl_type;
    new_op->next = operand;
    operand = new_op;
  }
}

Operand *pop_operand() {
  Operand *pop_op = (Operand *)malloc(sizeof(Operand));
  Operand *tmp = operand;
  if (!operand)
    return NULL;  
  // copy operand node
  *pop_op = *operand;
  pop_op->next = NULL;  // no next operand node
  // point to next operand node
  operand = operand->next;
  // release memory space
  free(tmp);
  return pop_op;
}

void push_operator(TokenType opr_type) {
  Operator *new_opr = (Operator *)malloc(sizeof(Operator));
  new_opr->opr_type = opr_type;
  new_opr->next = operator;
  operator= new_opr;
}

Operator *pop_operator() {
  Operator *pop_opr = (Operator *)malloc(sizeof(Operator));
  Operator *tmp = operator;
  if (!operator)
    return NULL;  
  // copy operator node
  pop_opr->opr_type = operator->opr_type;
  pop_opr->next = NULL;  // no next operator node
  // point to next operator node
  operator = operator->next;
  // release memory space
  free(tmp);
  return pop_opr;
}
