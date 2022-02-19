#ifndef TREE_H_
#define TREE_H_

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

struct tree {
   int prodrule;
   char *symbolname;
   int nkids;
   struct tree *kids[9]; /* if nkids >0 */
   struct token *leaf;   /* if nkids == 0; NULL for ε productions */
};

//jzero_main.c variables
char *filename;

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
//void free_list(struct tokenlist *head);
//void print_list(struct tokenlist *head);
void print_node(struct tree *tree);
void text_eval(struct token *node);

#endif
