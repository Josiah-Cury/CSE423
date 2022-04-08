#ifndef ERROR_H_
#define ERROR_H_

#include "tree.h"

void lex_error(char *s);
void semantic_error(char *s, struct tree *n);
int yyerror(const char *s);

#endif
