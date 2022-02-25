#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "tree.h"
#include "j0gram.tab.h"

int error_handler(int lex_code){
	switch(lex_code) {
		case NOT_RESERVED:
			printf("%s:%d: %s is not a valid reserved word in Jzero!\n", filename, yylineno, yytext);
			return -1;
		case CHAR_ERROR:
			printf("%s:%d: %s is not a valid character literal.\n", filename, yylineno, yytext);
			return -1;
		case UNKNOWN:
			printf("%s:%d: An unexpected token %s occured!\n", filename, yylineno, yytext);
			return -1;
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

	/*struct tree *new_tree = malloc(sizeof(struct tree));
	yylval.treeptr = new_tree;
	yylval.treeptr->prodrule = code;
	yylval.treeptr->nkids = 0;
	yylval.treeptr->symbolname = NULL;

	for(int i = 0; i < 9; i++){
		yylval.treeptr->kids[i] = NULL;
	}

	yylval.treeptr->leaf = NULL;*/
	yylval.treeptr = create_tree();
	yylval.treeptr->prodrule = TOKEN;
	yylval.treeptr->symbolname = "Token";
	yylval.treeptr->nkids = 0;

	yylval.treeptr->leaf = malloc(sizeof(struct token));
	yylval.treeptr->leaf->category = code;
	yylval.treeptr->leaf->text = strdup(yytext);
	yylval.treeptr->leaf->lineno = yylineno;
	yylval.treeptr->leaf->filename = filename;
	text_eval(yylval.treeptr->leaf);

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
			if(errno == ERANGE)
				printf("Range error occurred.\n");

			node->ival = l;
			node->dval = 0;
			node->sval = NULL;
			break;
		case FLOAT_LITERAL:
			step = node->text;
			f = strtof(step, NULL);
			if(errno == ERANGE)
				printf("Range error occurred.\n");

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
