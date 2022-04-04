#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "defs.h"
#include "j0gram.tab.h"
#include "error.h"
#include "symt.h"

extern int yydebug;

int main(int argc, char *argv[]) {

	if(argc == 1)
		yyin = stdin;

	while (--argc > 0) {

		if ((yyin = fopen(*++argv, "r")) == NULL)
    		return 0;

    	yyfilename = *argv;
    	yylineno = 1;

    	//yydebug = 1;
    	printf("Beginning parse of file: %s\n", yyfilename);
    	yyparse();
        print_tree(tree_root, 1);
		//free_tree(tree_root);
		printf("\n");

		printf("Beginning print of all symbols in files!\n");
		printsyms(tree_root);
		printf("\n");

		globals = new_st("global", 10);
		current = globals;
		printf("Populating symbol tables!\n");
		populate_symboltables(tree_root);
		printf("\n");

		printf("Printing symbol tables\n\n");
		printsymbols(globals, 0);
		printf("\n");

		delete_st(globals);
		printf("\n");
    }
    //printf("Category\t Text\t\t  Lineno   Filename\t\t Ival/Sval\n");
	//printf("------------------------------------------------------------------\n");

	fclose(yyin);
	yylex_destroy();
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
