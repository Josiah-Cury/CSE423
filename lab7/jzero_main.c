#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "defs.h"
#include "j0gram.tab.h"
#include "error.h"
#include "dot_tree.h"

extern int yydebug;

int main(int argc, char *argv[]) {

	if(argc == 1)
		yyin = stdin;

	while (--argc > 0) {

		if ((yyin = fopen(*++argv, "r")) == NULL)
    		return 0;

    	yyfilename = strrchr(*argv, '/') + 1;
    	printf("This is the filename: %s!\n", yyfilename);
    	yylineno = 1;

    	//yydebug = 1;
    	printf("Beginning parse of file: %s\n", yyfilename);
    	yyparse();
        print_tree(tree_root, 1);

		print_graph(tree_root, yyfilename);

		char buf[100];

		sprintf(buf, "dot -Tpng %s.dot > %s.png", yyfilename, yyfilename);
		printf("%s\n", buf);
		system(buf);
		//system("dot -Tpng HelloWorld.java.dot > HelloWorld.java.png");

		free_tree(tree_root);
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
