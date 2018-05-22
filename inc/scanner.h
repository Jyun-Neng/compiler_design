#ifndef _SCANNER_H_
#define _SCANNER_H_
#define MAXTOKENLEN 40
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char token[MAXTOKENLEN];

TokenType getToken();

#endif
