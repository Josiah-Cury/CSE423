#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

extern int error;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Please supply an input file.\n");
    } else {
        yyin = fopen(argv[1], "r");
        yylex();
        if (error == 1) {
            printf("There was a problem with the multiline comment.\n");
        }
        //printf("Max number of multiline comments: %d\n", max);
    }

    return 0;
}
