#ifndef DEFS_H_
#define DEFS_H_

//jzero.l variables
extern int rows, words, chars;
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yylineno;

//jzero_main.c global variables
char *yyfilename;
struct tree *tree_root;

//jzero_main.c function declarations
void check_extension(char *fname);
int errors;

extern int yylex_destroy();

#endif
