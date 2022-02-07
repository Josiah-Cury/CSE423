#include <stdio.h>

extern FILE *yyin;
extern int yylex();
extern int rows, words, chars;

// Comment

/*
	Multiline comment
	line number check
*/

int main(int argc, char *argv[]) {

	if(argc == 1)
		printf("Usage: ./lab2_1 [file_name] [file_name_2] ...\n");
	while (--argc > 0) {
		if ((yyin = fopen(*++argv, "r")) == NULL) {
			fclose(yyin);
	    	return 1;
	    } else {
	    	yylex();
    		printf("\t%d\t%d\t%d\n", rows, words, chars);
	    	fclose(yyin);
	    }
    }

    return 0;
}
