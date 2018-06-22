#include "debugger.h"

void warning(int line, char *warningmsg) {
  fprintf(stderr, "%s:%d:%d: warning: %s", sourcefile, line_no, line, warningmsg);
}

void error(int line, char *errmsg) {
  fprintf(stderr, "%s:%d:%d: error: %s", sourcefile, line_no, line, errmsg);
  ERROR++;
}
