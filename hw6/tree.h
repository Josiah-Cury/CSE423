#ifndef TREE_H_
#define TREE_H_

#include "prodrules.h"
#include "token.h"
#include "j0gram.tab.h"
#include "tac.h"


struct typeinfo;
typedef struct sym_table *SymbolTable;

struct tree {
   int prodrule;
   char *symbolname;
   int nkids;
   int id;
   SymbolTable symtab;
   struct typeinfo *type;
   struct instr *icode;
   struct addr *address, *first, *follow, *onTrue, *onFalse;
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
