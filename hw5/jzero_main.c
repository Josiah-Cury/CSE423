#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "defs.h"
#include "j0gram.tab.h"
#include "error.h"
#include "symt.h"
#include "dot_tree.h"

extern int yydebug;

int main(int argc, char *argv[]) {
	int dotFlag = 0;
	int symtabFlag = 0;

	if(argc < 2) {
		printf("Need an input file!\n");
		printf("Try ./j0 -h\n");
		exit(0);
	} else if (argc > 2) {
		while(argv[1][0] == '-'){
			if (!strcmp(argv[1]+1, "dot")) {
				dotFlag = 1;
				printf("Setting symtabFlag to %d.\n", dotFlag);
			} else if (!strcmp(argv[1]+1, "symtab")) {
				symtabFlag = 1;
				printf("Setting symtabFlag to %d.\n", symtabFlag);
			} else {
				printf("There is no %s flag option.\n", argv[1]+1);
				exit(0);
			}
			++argv;
			--argc;
		}
	}

	while (--argc > 0) {

		if (argv[1][0] == '-') {
			if (!strcmp(argv[1]+1, "h")) {
				printf("./j0 [input .java files] ...\n\n");
				printf("Possible flags are as follows:\n");
				printf("\t-symtab - Prints Symbol Tables.\n");
				printf("\t-dot - Creates dot file and png in dot directory.\n");
			} else {
				printf("The flags should go before input files.\n");
			}
			exit(0);
		}

		if ((yyin = fopen(*++argv, "r")) == NULL){
			printf("Cannot open file: %s\n", *argv);
    		return 0;
		}


    	//yyfilename = *argv;
		yyfilename = strrchr(*argv, '/') + 1;
		yylineno = 1;

    	//yydebug = 1;
    	printf("Beginning parse of file: %s\n", yyfilename);
    	yyparse();
        print_tree(tree_root, 1);
		//free_tree(tree_root);
		printf("\n");

		if (dotFlag) {
			char buf[100];

			print_graph(tree_root, yyfilename);
			sprintf(buf, "mv %s.dot ./dot/", yyfilename);
			system(buf);
			sprintf(buf, "dot -Tpng ./dot/%s.dot > ./dot/%s.png", yyfilename, yyfilename);
			//printf("%s\n", buf);
			system(buf);
		}

		globals = new_st("global", 10);
		current = globals;
		create_builtin_packages(globals);

		printf("Populating symbol tables!\n");
		populate_symboltables(tree_root);
		printf("\n");

		if (symtabFlag) {
			printf("Beginning print of all symbols in files!\n");
			printsyms(tree_root);
			printf("\n");

			printf("Printing symbol tables\n\n");
			printsymbols(globals, 0);
			printf("\n");

			delete_st(globals);
			printf("\n");
		}
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
