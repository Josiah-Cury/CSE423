#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "defs.h"

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
	struct token *new_node = malloc(sizeof(struct token));
	yytoken = new_node;
	yytoken->category = code;
	yytoken->text = strdup(yytext);
	yytoken->lineno = yylineno;
	yytoken->filename = filename;
	text_eval(yytoken);
	return code;
}

void free_list(struct tokenlist *head){
	struct tokenlist *node;
	
	while(head != NULL){
		node = head;
		head = head->next;
		if(node->t->sval != NULL) {
			free(node->t->sval);
		}
		free(node->t->text);
		free(node->t);
		free(node);
	}
}

void print_list(struct tokenlist *head){
	struct tokenlist *node;
	
	node = head;
	
	while(node != NULL){
		if(node->t->category == STRING_LITERAL){
			printf("%d\t\t %-16s %-8d %-22s%s\n", \
			node->t->category, node->t->text, node->t->lineno, node->t->filename,\
			node->t->sval);
		} else if (node->t->category == CHARACTER_LITERAL) {
			printf("%d\t\t %-16s %-8d %-22s%c\n", \
			node->t->category, node->t->text, node->t->lineno, node->t->filename,\
			node->t->ival);
		} else if (node->t->category == INTEGER_LITERAL) {
			printf("%d\t\t %-16s %-8d %-22s%d\n", \
			node->t->category, node->t->text, node->t->lineno, node->t->filename,\
			node->t->ival);
		} else if (node->t->category == FLOAT_LITERAL) {
			printf("%d\t\t %-16s %-8d %-22s%f\n", \
			node->t->category, node->t->text, node->t->lineno, node->t->filename,\
			node->t->dval);
		} else {
			printf("%d\t\t %-16s %-8d %-14s\n", \
			node->t->category, node->t->text, node->t->lineno, node->t->filename);
		}
		node = node->next;
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
