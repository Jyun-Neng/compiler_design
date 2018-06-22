#include "cgen.h"

static char *dcl_name;
static int label_cnt = 0;

static void emit_prologue();
static void emit_includes();
static void emit_epilogue();
static void emit_data_segment();
static void emit_add(Operand *op1, Operand *op2);
static void emit_sub(Operand *op1, Operand *op2);
static void emit_mul(Operand *op1, Operand *op2);
static void emit_div(Operand *op1, Operand *op2);
static void emit_mod(Operand *op1, Operand *op2);
static void emit_assign(Operand *op1, Operand *op2);
static void emit_print(Operand *op1);
static void emit_println(Operand *op1);
static void emit_cmp(char *cmp, Operand *op1, Operand *op2);
static void emit_label(int label_cnt);

void cgen(char operation) {
  switch (operation) {
    case 'H':  // generate header code
      emit_prologue();
      break;
    case 'E':  // generate end code
      emit_epilogue();
      break;
    default:
      fprintf(stdout, "code generation error!\n");
      exit(1);
  }
}

void codegen(Operator *opr, Operand *op1, Operand *op2) {
  if (opr && op1) {
    switch (opr->opr_type) {
      case ASSIGN:
        emit_assign(op1, op2);
        break;
      case PLUS:
        emit_add(op1, op2);
        break;
      case MINUS:
        emit_sub(op1, op2);
        break;
      case TIMES:
        emit_mul(op1, op2);
        break;
      case OVER:
        emit_div(op1, op2);
        break;
      case MODULO:
        emit_mod(op1, op2);
        break;
      case PRINT:
        emit_print(op1);
        break;
      case PRINTLN:
        emit_println(op1);
        break;
      case GE:
        emit_cmp ("JGE", op1, op2);
        break;  
      case GT:
        emit_cmp ("JG", op1, op2);
        break;  
      case LE:
        emit_cmp ("JLE", op1, op2);
        break;  
      case LT:
        emit_cmp ("JL", op1, op2);
        break;  
      case EQ:
        emit_cmp ("JE", op1, op2);
        break;  
      case NE:
        emit_cmp ("JNE", op1, op2);
        break;  
      default:
        fprintf(stderr, "%s:%d:%d: error: unexpected expression\n", sourcefile,
                line_no, __LINE__);
        ERROR++;
        break;
    }
    free(opr);
  }
}

static void emit_prologue() {
  fprintf(code, "TITLE      %s\n", codefile);
  emit_includes();
  fprintf(code, ".STACK 2048\n");
  fprintf(code, ".CODE\n");
  fprintf(code, "\t_main PROC\n");
}

static void emit_includes() {
  fprintf(code, "INCLUDE    Irvine32.inc\n");
  fprintf(code, "INCLUDELIB Irvine32.lib\n");
  fprintf(code, "INCLUDELIB Kernel32.lib\n");
  fprintf(code, "INCLUDELIB User32.lib\n");
}

static void emit_label(int label_cnt) {
  fprintf(code, "_L%d:\n", label_cnt);
}

static void emit_epilogue() {
  fprintf(code, "\tCALL CRLF\n");
  fprintf(code, "\tINT 3\n");
  fprintf(code, "\t      exit\n");
  fprintf(code, "\t_main ENDP\n");
  emit_data_segment();
  fprintf(code, "\t      END _main\n");
}

static void emit_data_segment() {
  // st_print();
  fprintf(code, ".DATA\n");
  fprintf(code, "\t_SID  DB \"JyunNeng\"\n");
  Symbol *sym = st_pop();
  while (sym) {
    switch (sym->dcl_type) {
      case NUM:
        if (sym->n == 0)
          fprintf(code, "\t%s  DD %d\n", sym->dcl_name, sym->dcl_val.num);
        else
          fprintf(code, "\t%s%d  DD %d\n", sym->dcl_name, sym->n,
                  sym->dcl_val.num);
        break;
      case BOOLEAN:
        if (sym->n == 0)
          fprintf(code, "\t%s  DB %d\n", sym->dcl_name, sym->dcl_val.num);
        else
          fprintf(code, "\t%s%d  DB %d\n", sym->dcl_name, sym->n,
                  sym->dcl_val.num);
        break;
      case STRTYPE:
        if (sym->n == 0)
          fprintf(code, "\t%s  DB \"%s\", 0\n", sym->dcl_name,
                  sym->dcl_val.str);
        else
          fprintf(code, "\t%s%d  DB \"%s\", 0\n", sym->dcl_name, sym->n,
                  sym->dcl_val.str);
        break;
      case CHARTYPE:
        if (sym->n == 0)
          fprintf(code, "\t%s  DB \'%s\'\n", sym->dcl_name, sym->dcl_val.str);
        else
          fprintf(code, "\t%s%d  DB \'%s\', 0\n", sym->dcl_name, sym->n,
                  sym->dcl_val.str);
        break;
      default:
        fprintf(stderr, "%s:%d:%d: warning: unknown symbol type \"%s\"\n",
                sourcefile, line_no, __LINE__, sym->dcl_name);
        ERROR++;
        break;
    }
    free(sym);
    sym = st_pop();
  }
}

static void emit_assign(Operand *op1, Operand *op2) {
  if (op1->op_type != op2->op_type)
    fprintf(stdout, "%d, assigned type error!\n", line_no);
  if (op1->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op1->op_name);
  fprintf(code, "\tMOV %s, EAX\n", op2->op_name);
  free(op1);
  free(op2);
}

static void emit_add(Operand *op1, Operand *op2) {
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, NUM);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op2->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op2->op_name);
  if (op1->n != 0)
    fprintf(code, "\tADD EAX, %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tADD EAX, %s\n", op1->op_name);
  fprintf(code, "\tMOV %s%d, EAX\n", result->dcl_name, result->n);
  push_operand(result);
  free(op1);
  free(op2);
}

static void emit_sub(Operand *op1, Operand *op2) {
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, NUM);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op1->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op1->op_name);
  if (op2->n != 0)
    fprintf(code, "\tSUB EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tSUB EAX, %s\n", op2->op_name);
  fprintf(code, "\tMOV %s%d, EAX\n", result->dcl_name, result->n);
  push_operand(result);
  free(op1);
  free(op2);
}

static void emit_mul(Operand *op1, Operand *op2) {
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, NUM);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op2->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op2->op_name);
  if (op1->n != 0)
    fprintf(code, "\tIMUL %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tIMUL %s\n", op1->op_name);
  fprintf(code, "\tMOV %s%d, EAX\n", result->dcl_name, result->n);
  push_operand(result);
  free(op1);
  free(op2);
}
static void emit_div(Operand *op1, Operand *op2) {
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, NUM);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op2->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op2->op_name);
  fprintf(code, "\tCDQ\n");
  if (op1->n != 0)
    fprintf(code, "\tIDIV %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tIDIV %s\n", op1->op_name);
  fprintf(code, "\tMOV %s%d, EAX\n", result->dcl_name, result->n);
  push_operand(result);
  free(op1);
  free(op2);
}
static void emit_mod(Operand *op1, Operand *op2) {
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, NUM);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op2->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op2->op_name);
  fprintf(code, "\tCDQ\n");
  if (op1->n != 0)
    fprintf(code, "\tIDIV %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tIDIV %s\n", op1->op_name);
  fprintf(code, "\tMOV %s%d, EDX\n", result->dcl_name, result->n);
  push_operand(result);
  free(op1);
  free(op2);
}

static void emit_print(Operand *op1) {
  switch (op1->op_type) {
    case NUM:
      if (op1->n != 0)
        fprintf(code, "\tMOV EAX, %s%d\n", op1->op_name, op1->n);
      else
        fprintf(code, "\tMOV EAX, %s\n", op1->op_name);
      fprintf(code, "\tCALL WriteInt\n");
      break;
    case STRTYPE:
      if (op1->n != 0)
        fprintf(code, "\tMOV EDX, offset %s%d\n", op1->op_name, op1->n);
      else
        fprintf(code, "\tMOV EDX, offset %s\n", op1->op_name);
      fprintf(code, "\tCALL WriteString\n");
      break;
    default:
      break;
  }
  free(op1);
}

static void emit_println(Operand *op1) {
  switch (op1->op_type) {
    case NUM:
      if (op1->n != 0)
        fprintf(code, "\tMOV EAX, %s%d\n", op1->op_name, op1->n);
      else
        fprintf(code, "\tMOV EAX, %s\n", op1->op_name);
      fprintf(code, "\tCALL WriteInt\n");
      break;
    case STRTYPE:
      if (op1->n != 0)
        fprintf(code, "\tMOV EDX, offset %s%d\n", op1->op_name, op1->n);
      else
        fprintf(code, "\tMOV EDX, offset %s\n", op1->op_name);
      fprintf(code, "\tCALL WriteString\n");
      break;
    default:
      break;
  }
  free(op1);
  fprintf(code, "\tCALL CRLF\n");
}

static void emit_cmp(char *cmp, Operand *op1, Operand *op2) {
  int tmp_cnt = label_cnt;  
  Symbol *result = malloc(sizeof(Symbol));
  char *val = "0";
  dcl_name = (char *)malloc(MAXTOKENLEN * sizeof(char));
  dcl_name = "_S";
  st_insert(dcl_name, ++dcl_n, val, BOOLEAN);
  result = st_lookup(dcl_name, dcl_n);
  if (op1->op_type != NUM && op2->op_type != NUM)
    fprintf(stdout, "%d, operation type error!\n", line_no);

  if (op2->n != 0)
    fprintf(code, "\tMOV EAX, %s%d\n", op2->op_name, op2->n);
  else
    fprintf(code, "\tMOV EAX, %s\n", op2->op_name);
  if (op1->n != 0)
    fprintf(code, "\tCMP EAX, %s%d\n", op1->op_name, op1->n);
  else
    fprintf(code, "\tCMP EAX, %s\n", op1->op_name);
  fprintf(code, "\t%s _L%d\n", cmp, label_cnt);
  fprintf(code, "\tMOV EBX,  0\n");
  fprintf(code, "\tJMP _L%d\n", ++label_cnt);
  emit_label(tmp_cnt);
  fprintf(code, "\tMOV EBX,  1\n");
  emit_label(label_cnt++);
  fprintf(code, "\tMOV %s%d, EBX\n", result->dcl_name, result->n);
  push_operand(result);

  free(op1);
  free(op2);
}
