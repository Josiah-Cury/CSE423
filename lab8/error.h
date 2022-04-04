#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "defs.h"

void lex_error(char *s);
void semantic_error(char *s);
int yyerror(const char *s);

#endif
