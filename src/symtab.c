#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n = 0;
void st_init() { 
  sym = NULL; 
}

void op_init() {
  operand = NULL;
  operator = NULL;
}

void st_insert(char *dcl_name, char *dcl_char, TokenType dcl_type) {
  Symbol *new_sym = malloc(sizeof(Symbol));
  Symbol *find_sym = malloc(sizeof(Symbol));
  new_sym->dcl_name = malloc(sizeof(char));
  if (!dcl_name) {
    dcl_name = malloc(sizeof(char));
    new_sym->dcl_name = "_S";
    new_sym->n = ++n;
  }
  else {
    new_sym->dcl_name = dcl_name;
    new_sym->n = 0;
  }
  new_sym->dcl_type = dcl_type;
  switch (dcl_type) {
    case STRTYPE:
    case CHARTYPE:
      new_sym->dcl_val.str = malloc(sizeof(char));
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
    case ID:
      find_sym = st_lookup(dcl_char);
      new_sym->dcl_val = find_sym->dcl_val;
      new_sym->dcl_type = find_sym->dcl_type;
      break;
    default:
      break;
  }
  new_sym->next = sym;
  sym = new_sym;
}

Symbol *st_lookup(char *srch_name) {
  Symbol *sym_traverse = malloc(sizeof(Symbol));
  sym_traverse = sym;
  while (sym_traverse) {
    if (strcmp(sym_traverse->dcl_name, srch_name) == 0) {  // match
      return sym_traverse;
    }
    sym_traverse = sym_traverse->next;
  }
  fprintf(stderr, "symbol not found!\n");
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
  Operand *new_op = malloc(sizeof(Operand));  
  new_op->op_name = malloc(sizeof(char));
  new_op->op_name = op->dcl_name;
  new_op->n = op->n;
  new_op->op_type = op->dcl_type;
  new_op->next = operand;
  operand = new_op;
}

Operand *pop_operand() {
  Operand *pop_op = malloc(sizeof(Operand));
  pop_op->op_name = malloc(sizeof(char));
  strcpy(pop_op->op_name, operand->op_name);
  pop_op->op_type = operand->op_type;  
  pop_op->n = operand->n;
  operand = operand->next;
  return pop_op;
}

void operand_traversal() {
  Operand *traversal_op = operand;
  while(traversal_op) {
    printf("%s ", traversal_op->op_name);
    traversal_op =  traversal_op->next;
  }
  printf("\n");
}

void push_operator(TokenType opr_type) {
  Operator *new_opr = malloc(sizeof(Operator));
  new_opr->opr_type = opr_type;
  new_opr->next = operator;
  operator = new_opr;
}

Operator *pop_operator() {
  Operator *pop_opr = malloc(sizeof(Operator));
  pop_opr->opr_type = operator->opr_type;
  operator = operator->next;
  return pop_opr;
}
