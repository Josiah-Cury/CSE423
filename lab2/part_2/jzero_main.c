#include <stdio.h>
#include "defs.h"

//extern int rows, words, chars;


// Comment

/*
	
	Multiline comment test:
		Does it work?
*/

int main(int argc, char *argv[]) {

	int lex_code;

	if(argc == 1)
		yyin = stdin;
	while (--argc > 0) {
		if ((yyin = fopen(*++argv, "r")) == NULL) {
	    	return 1;
	    } else {
	    	while((lex_code = yylex()) != 0){
	    		switch(lex_code) {
	    			case 500:
	    				printf("Error 500: The word %s is not a valid reserved word in Jzero!\n", yytext);
	    				return -1;
	    				break;
	    			case 501:
	    				printf("Error 501: There is an invalid char literal %s occured!\n", yytext);
	    				return -1;
	    				break;
	    			case 502:
	    				printf("Error 502: An unexpected token %s occured!\n", yytext);
	    				return -1;
	    				break;
					default:
						printf("Return Code: %d\tWord: %s\n", lex_code, yytext);
						break;
	    		}
	    		
	    	}
    		//printf("\nRows: %d\tWords: %d\tChars:%d\n", rows, words, chars);
	    	fclose(yyin);
	    }
    }

    return 0;
}
