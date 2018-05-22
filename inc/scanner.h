#ifndef _SCANNER_H_
#define _SCANNER_H_
#define MAXTOKENLEN 40
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char token[MAXTOKENLEN];

TokenType getToken();

#endif
