#ifndef _SCANNER_H_
#define _SCANNER_H_
#define MAXTOKENLEN 40
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

char token[MAXTOKENLEN];

// Return the type of next token in source file.
TokenType getToken();

#endif
