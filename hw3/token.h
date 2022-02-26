#ifndef TOKEN_H_
#define TOKEN_H_

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

//token.c Function Declarations
int error_handler(int lex_code);
int init_token(int code);
void print_node(struct tree *tree);
void text_eval(struct token *node);

//jzero_main.c function declarations
void check_extension(char *fname);




#endif
