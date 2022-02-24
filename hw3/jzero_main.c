#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "j0gram.tab.h"

int yyerror(char *s) {
   fprintf(stderr, "%s\n", s); exit(1);
}

extern int yydebug;

int main(int argc, char *argv[]) {

	if(argc == 1)
		yyin = stdin;
		
	while (--argc > 0) {
	
		if ((yyin = fopen(*++argv, "r")) == NULL)
    		return 0;
		
    	filename = *argv;
    	yylineno = 1;
    	
    	//yydebug = 1;
    	yyparse();
    }
    printf("Category\t Text\t\t  Lineno   Filename\t\t Ival/Sval\n");
	printf("------------------------------------------------------------------\n");
    
	fclose(yyin);
    return 0;
}

void check_extension(char *fname) {
	int len = strlen(fname);
	//printf("%d\n", len);
	
	if(len <= 5 || strcmp(fname+len-5, ".java") != 0) {
		strcat(fname, ".java");
		//printf("%s\n", fname);
	}
}
