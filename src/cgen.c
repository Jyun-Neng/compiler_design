#include "cgen.h"

static void emit_prologue();
static void emit_includes();
static void emit_epilogue();
static void emit_data_segment();

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
  fprintf(code, "INCLUDELIB Kernal32.lib\n");
  fprintf(code, "INCLUDELIB User32.lib\n");
}

static void emit_epilogue() {
  fprintf(code, "\t      exit\n");
  fprintf(code, "\t_main ENDP\n");
  emit_data_segment();
  fprintf(code, "\t      END _main\n");
}

static void emit_data_segment() {
  fprintf(code, ".DATA\n");
  fprintf(code, "\t_SID  DB \"JyunNeng\"\n");
}
