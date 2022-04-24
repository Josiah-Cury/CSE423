#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "token.h"
#include "defs.h"
#include "error.h"

int error_handler(int lex_code){
	switch(lex_code) {
		case NOT_RESERVED:
			lex_error("Not a reserved word!");
		case CHAR_ERROR:
			lex_error("Not a void character literal!");
		case UNKNOWN:
			lex_error("Unknown token!");
		case STRING_ERROR:
			lex_error("Open ended string!");
		default:

			break;
	}
	return 0;
}

int init_token(int code){
	/*int lex_code;

	if(error_handler(lex_code) == -1){
		free_list(head);
		fclose(yyin);
		return -1;
	}*/

	yylval.treeptr = create_tree();
	yylval.treeptr->prodrule = TOKEN;
	yylval.treeptr->symbolname = "Token";
	yylval.treeptr->type = NULL;
	yylval.treeptr->nkids = 0;

	yylval.treeptr->leaf = malloc(sizeof(struct token));
	yylval.treeptr->leaf->category = code;
	yylval.treeptr->leaf->text = strdup(yytext);
	yylval.treeptr->leaf->lineno = yylineno;
	yylval.treeptr->leaf->filename = yyfilename;
	text_eval(yylval.treeptr->leaf);

	error_handler(yylval.treeptr->leaf->category);
	//print_node(yylval.treeptr);

	return code;
}

void print_node(struct tree *tree){
	struct tree *node;

	node = tree;

	if(node->leaf->category == STRING_LITERAL){
		printf("%d\t\t %-16s %-8d %-22s%s\n", \
		node->leaf->category, node->leaf->text, node->leaf->lineno, node->leaf->filename,\
		node->leaf->sval);
	} else if (node->leaf->category == CHARACTER_LITERAL) {
		printf("%d\t\t %-16s %-8d %-22s%c\n", \
		node->leaf->category, node->leaf->text, node->leaf->lineno, node->leaf->filename,\
		node->leaf->ival);
	} else if (node->leaf->category == INTEGER_LITERAL) {
		printf("%d\t\t %-16s %-8d %-22s%d\n", \
		node->leaf->category, node->leaf->text, node->leaf->lineno, node->leaf->filename,\
		node->leaf->ival);
	} else if (node->leaf->category == FLOAT_LITERAL) {
		printf("%d\t\t %-16s %-8d %-22s%f\n", \
		node->leaf->category, node->leaf->text, node->leaf->lineno, node->leaf->filename,\
		node->leaf->dval);
	} else {
		printf("%d\t\t %-16s %-8d %-14s\n", \
		node->leaf->category, node->leaf->text, node->leaf->lineno, node->leaf->filename);
	}

}

void text_eval(struct token *node){
	int len = strlen(node->text);
	char buf[len+1];
	char *step = NULL;
	long l = 0;
	float f = 0.0;
	int i, j;
	errno = 0;

	switch(node->category) {
		case INTEGER_LITERAL:
			step = node->text;
			l = strtol(step, NULL, 0);
			if(l < -2147483648 || l > 2147483647){
				printf("Range error occurred.\n");
				exit(1);
			}
			node->ival = l;
			node->dval = 0;
			node->sval = NULL;
			break;
		case FLOAT_LITERAL:
			step = node->text;
			f = strtof(step, NULL);
			if(errno == ERANGE){
				printf("Range error occurred.\n");
				exit(1);
			}
			node->ival = 0;
			node->dval = f;
			node->sval = NULL;
			break;
		case CHARACTER_LITERAL:
			step = node->text;
			for(i=0, j=0; i<len; i++,j++){
				if(step[i] == '\'')
					i++;
				if(step[i] == '\\'){
					switch(step[++i]){
						case 'n':
							buf[j] = '\n';
							break;
						case 't':
							buf[j] = '\t';
							break;
						case '\\':
							buf[j] = '\\';
							break;
						case '"':
							buf[j] = '"';
							break;
						case '\'':
							buf[j] = '\'';
							break;
						default:
							buf[j] = step[--i];
							buf[++j] = step[++i];
							break;
					}
				} else {
					buf[j] = step[i];
				}
			}
			buf[j] = '\0';
			node->ival = buf[0];
			node->dval = 0;
			node->sval = NULL;
			break;
		case STRING_LITERAL:
			step = node->text;
			for(i=0, j=0; i<len; i++,j++){
				if(step[i] == '"')
					i++;
				if(step[i] == '\\'){
					switch(step[++i]){
						case 'n':
							buf[j] = '\n';
							break;
						case 't':
							buf[j] = '\t';
							break;
						case '\\':
							buf[j] = '\\';
							break;
						case '"':
							buf[j] = '"';
							break;
						case '\'':
							buf[j] = '\'';
							break;
						default:
							buf[j] = step[--i];
							buf[++j] = step[++i];
							break;
					}
				} else {
					buf[j] = step[i];
				}
			}
			buf[j] = '\0';

			//printf("%s\n", buf);

			node->sval = strdup(buf);

			node->ival = 0;
			node->dval = 0;

			//free(buf);
			break;
		default:
			node->ival = 0;
			node->dval = 0;
			node->sval = NULL;
			break;
	}
}
