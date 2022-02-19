#include <stdio.h>
#include <stdlib.h>
#include "ns.tab.h"
extern FILE *yyin;

int yyerror(char *s);

int main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	printf("yyparse() returns %d\n", yyparse());
	return 0;
}

int yyerror(char *s) {
   fprintf(stderr, "%s\n", s); exit(1);
}
