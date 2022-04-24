#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


int serial = 0;

struct tree *link_tree(int prodrule, char *symbolname, int nkids, ...) {
    va_list arg;
    va_start(arg, nkids);

    struct tree *linked_tree = create_tree();

    linked_tree->prodrule = prodrule;
    linked_tree->symbolname = symbolname;
    linked_tree->nkids = nkids;
	linked_tree->id = serial++;
	linked_tree->type = NULL;
	linked_tree->icode = NULL;
	linked_tree->address = NULL;
	linked_tree->symtab = NULL;

    for(int i = 0; i < nkids; i++){
        linked_tree->kids[i] = va_arg(arg, struct tree *);
		if(linked_tree->kids[i])
			linked_tree->kids[i]->id = serial++;
        //printf("Made into kids loop!");
    }

    va_end(arg);

    return linked_tree;
}

struct tree *create_tree() {
    struct tree *new_tree = malloc(sizeof(struct tree));
    memset(new_tree, 0, sizeof(struct tree));
    return new_tree;
}

void free_tree(struct tree *root){
    if(root == NULL)
        return;
    for(int i = 0; i < 9; i++){
        free_tree(root->kids[i]);
    }

    if(root->leaf != NULL){
        free(root->leaf->text);
        if(root->leaf->sval != NULL){
            free(root->leaf->sval);
        }
        free(root->leaf);
    }

    free(root);
}

int print_tree(struct tree *root, int depth) {
    int i;
    if(root == NULL){
        //printf("Node is null!");
        return 1;
    }
    if (root->prodrule == TOKEN){
        printf("%*s %s %d: %s\n", depth*4, " ", root->symbolname, root->leaf->category, root->leaf->text);
    } else {
        printf("%*s %s: %d\n", depth*4, " ", root->symbolname, root->nkids);
    }

    for(i=0; i < root->nkids; i++){
        print_tree(root->kids[i], depth+1);
    }

    return 0;
}
