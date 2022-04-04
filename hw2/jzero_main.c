#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

int main(int argc, char *argv[]) {

	int lex_code;
	struct tokenlist *head = NULL, *node = NULL, *tail = NULL;

	if(argc == 1)
		yyin = stdin;
	
	
	while (--argc > 0) {
	
		if ((yyin = fopen(*++argv, "r")) == NULL)
    		return 0;
		
    	filename = *argv;
    	yylineno = 1;
    	
    	while((lex_code = yylex()) != 0){
    		if(error_handler(lex_code) == -1){
    			free_list(head);
    			fclose(yyin);
    			return 0;
			} else {
				node = malloc(sizeof(struct tokenlist));
				node->t = NULL;
				node->next = NULL;
				check_extension(yytoken->filename);

				node->t = yytoken;
				if(head == NULL) {
					head = node;
					tail = node;
				} else if (head == tail) {
					tail = node;
					head->next = tail;
				} else {
					tail->next = node;
					tail = node;
				}
			}
    	}
    }
    printf("Category\t Text\t\t  Lineno   Filename\t\t Ival/Sval\n");
	printf("------------------------------------------------------------------\n");
    print_list(head);
	free_list(head);
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
