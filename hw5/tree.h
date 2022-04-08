#ifndef TREE_H_
#define TREE_H_

#include "prodrules.h"
#include "token.h"
#include "j0gram.tab.h"

struct typeinfo;

struct tree {
   int prodrule;
   char *symbolname;
   int nkids;
   int id;
   struct typeinfo *type;
   struct tree *kids[9]; /* if nkids >0 */
   struct token *leaf;   /* if nkids == 0; NULL for ε productions */
};

//tree.c Function Declarations
struct tree *link_tree(int prodrule, char *symbolname, int nkids, ...);
struct tree *create_tree();

void print_node(struct tree *tree);
int print_tree(struct tree *root, int depth);
void free_tree(struct tree *root);

#endif
