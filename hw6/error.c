#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "error.h"
#include "tree.h"
#include "defs.h"


int yyerror(const char *s) {
	if(strcmp(s, "lexical error") == 0){
		fprintf(stderr, "%s:%d %s for '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(1);
	} else if (strcmp(s, "semantic error") == 0) {
		//fprintf(stderr, "%s:%d %s for '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(3);
	} else {
		fprintf(stderr, "%s:%d %s before '%s' token\n", yyfilename, yylineno, s, yytext);
		exit(2);
	}
}

void lex_error(char *s) {
    fprintf(stderr, "%s\n", s);
    yyerror("lexical error");
}

void jzero_error(char *s) {
	fprintf(stderr, "This is not a valid j0.1 feature: %s\n", s);
	exit(2);
}

void semantic_error(char *s, struct tree *n) {
	fprintf(stderr, "%s\n", s);

	//while (n && (n->nkids > 0)) n=n->kids[0];
    //if (n) fprintf(stderr, "%s:%d: ", n->leaf->filename, n->leaf->lineno);
	//if (n && n->prodrule == PR_IDENT) fprintf(stderr, " %s", n->leaf->text);
	//fprintf(stderr, "\n");

	fprintf(stderr, "%s:%d: ", n->leaf->filename, n->leaf->lineno);
	fprintf(stderr, " '%s'", n->leaf->text);
	fprintf(stderr, "\n");
	//errors++;

	yyerror("semantic error");
}
