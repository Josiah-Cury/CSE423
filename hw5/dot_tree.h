#ifndef DOT_TREE_H_
#define DOT_TREE_H_

#include "tree.h"

void print_graph(struct tree *root, char *filename);
int print_graph_node(struct tree *node, FILE *fp);

#endif
