#include "dot_tree.h"

#include <stdlib.h>
#include <string.h>

void print_graph(struct tree *root, char *filename) {
	FILE *fp = NULL;
	char *dotfile, *dotextension = ".dot";

	dotfile = strdup(filename);

	char *completedot = malloc(strlen(dotfile)+strlen(dotextension)+1);

	strcpy(completedot, dotfile);
	strcat(completedot, dotextension);

	//printf("This is the dotfile location %s\n", completedot);
	fp = fopen(completedot, "w");
	if (fp == NULL) printf("This dot file pointer is NULL.\n");

	fprintf(fp, "digraph {\n");
	print_graph_node(root, fp);
	fprintf(fp, "}");


	free(dotfile);
	free(completedot);
	fclose(fp);
}

int print_graph_node(struct tree *node, FILE *fp){
	if(node == NULL){
        //printf("Node is null!");
        return 1;
    }
    int i;

    if (node->prodrule == TOKEN){
		i = strlen(node->leaf->text)-1;
		if(node->leaf->text[0] == '"' && node->leaf->text[i] == '"'){
			//printf("This starts and ends with a quote: %s\n", node->leaf->text);
			char subtext[i];
			strncpy(subtext, &node->leaf->text[1], i-1);
			subtext[i-1] = '\0';
			//printf("The is the string with the quotes removed: %s\n", subtext);
			fprintf(fp, "\t%d [label=\"%s\"]\n", node->id, subtext);
		} else {
        	fprintf(fp, "\t%d [label=\"%s\"]\n", node->id, node->leaf->text);
        	//printf("id=%d: symbolname=%s: lexeme=%s\n", node->id, node->symbolname, node->leaf->text);
        }
    } else {
		//if (node->prodrule != PR_IDENT) {
			fprintf(fp, "\t%d [shape = rectangle, label=\"%s\"]\n", node->id, node->symbolname);
		//}
        //printf("id=%d: symbolname=%s\n", node->id, node->symbolname);
    }

	if (node->nkids != 0) {
		for(i=0; i < node->nkids; i++){
        	print_graph_node(node->kids[i], fp);
    	}
    	fprintf(fp, "\t%d -> { ", node->id);
    	for(i=0;i < node->nkids; i++){
    		if(node->kids[i] != NULL) {fprintf(fp, "%d ", node->kids[i]->id);}
    	}
    	fprintf(fp, "}\n");
	}

	return 0;
}
