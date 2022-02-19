#ifndef JZERO_H_
#define JZERO_H_

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

/*
//	Jzero Default Word Code
#define WORD_CODE 		257

//	Jzero Reserved Words
#define BOOLEAN                 258
#define BREAK                   259
#define CASE                    260
#define CHAR                    261
#define CLASS                   262
#define CONTINUE                263
#define DEFAULT                 264
#define DOUBLE                  265
#define ELSE                    266
#define FLOAT                   267
#define FOR                     268
#define IF                      269
//#define INSTANCEOF              270
#define INT                     271
#define LONG                    272
#define NEW                     273
#define PUBLIC                  274
#define RETURN                  275
#define STATIC                  276
#define SWITCH                  277
#define VOID                    278
#define WHILE                   279

//	Jzero Operators
#define ASSIGN                  300
#define ADD                     301
#define SUB                     302
#define MULTIPLY                303
#define DIVIDE                  304
#define MODULO                  305
#define INCREMENT               306
#define DECREMENT               307
#define NEGATION                308
#define EQUAL                   309
#define NOT_EQUAL               310
#define GREATER_THAN            311
#define LESS_THAN               312
#define GREATER_EQUAL           313
#define LESS_EQUAL              314
#define LOGIC_AND               315
#define LOGIC_OR                316
#define LOGIC_NOT               317
#define INSTANCEOF	          	318
#define LEFT_SUBSCRIPT          319
#define RIGHT_SUBSCRIPT         320
#define DOT                     321
#define TYPE_CAST               322

//	Jzero Punctuation
#define LEFT_PARENTHESIS        350
#define RIGHT_PARENTHESIS       351
#define COMMA                   352
#define SEMI_COLON              353
#define LEFT_BRACKET            354
#define RIGHT_BRACKET           355
#define COLON                   356

//	Jzero Literal
#define INTEGER_LITERAL         400
#define FLOAT_LITERAL           401
#define CHARACTER_LITERAL       402
#define STRING_LITERAL          403

//	Jzero Error
#define NOT_RESERVED            500
#define CHAR_ERROR				501
#define UNKNOWN					502
#define INT_ERROR				503
*/
#endif
