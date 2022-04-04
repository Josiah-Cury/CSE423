#include "error.h"

int yyerror(const char *s) {
	if(strcmp(s, "lexical error") == 0){
		fprintf(stderr, "%s:%d %s for '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(1);
	} else if (strcmp(s, "semantic error") == 0) {
		fprintf(stderr, "%s:%d %s for '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(3);
	} else {
		fprintf(stderr, "%s:%d %s before '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(2);
	}
}

void lex_error(char *s) {
    printf("%s\n", s);
    yyerror("lexical error");
}

void semantic_error(char *s) {
	printf("%s\n", s);
	yyerror("semantic error");
}
