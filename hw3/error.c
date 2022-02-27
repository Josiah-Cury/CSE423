#include "error.h"

int yyerror(char *s) {
   fprintf(stderr, "%s:%d %s before '%s' token\n", yyfilename, yylineno, s, yytext);
   exit(1);
}

void lex_error(char *s) {
    printf("%s ", s);
    yyerror("lexical error");
}
