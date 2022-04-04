#ifndef DEFS_H_
#define DEFS_H_

//Token structures
struct token {
	int category;   /* the integer code returned by yylex */
	char *text;     /* the actual string (lexeme) matched */
	int lineno;     /* the line number on which the token occurs */
	char *filename; /* the source file in which the token occurs */
	int ival;       /* for integer constants, store binary value here */
	double dval;	   /* for real constants, store binary value here */
	char *sval;     /* for string constants, malloc space, de-escape, store */
		           /*    the string (less quotes and after escapes) here */
};

struct tokenlist {
	struct token *t;
	struct tokenlist *next;
};

//jzero_main.c variables
char *filename;
struct token *yytoken;
struct tokenlist *yytokenlist, *head, *tail;

//jzero_main.c function declarations
void check_extension(char *fname);

//jzero.l variables
extern int rows, words, chars;
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yylineno;


//token.c Function Declarations
int error_handler(int lex_code);
int init_token(int code);
void free_list(struct tokenlist *head);
void print_list(struct tokenlist *head);
void text_eval(struct token *node);

#endif
