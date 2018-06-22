#ifndef _CGEN_H_
#define _CGEN_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "symtab.h"
#include "debugger.h"

void cgen(char operation);
void codegen(Operator *opr, Operand *op1, Operand *op2); 
    
#endif
