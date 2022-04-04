#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct token {
    int category;
    char *lexeme;
    int lineno;
};

int main()
{

    struct token *leaf;

    leaf = malloc(sizeof(struct token));

    memset(leaf, 0, sizeof(struct token));

    printf("This is what is stored in lead\nleaf->category: %d\nleaf->lexeme: \
    %s\nleaf->lineno:%d\n", leaf->category, leaf->lexeme, leaf->lineno);

    return 0;
}
