#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_
#include <stdio.h>
#include "global.h"

void warning(int line, char *warningmsg);
void error(int line, char *errmsg);

#endif
