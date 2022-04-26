#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "symt.h"

static const char *typenames[] = {
	"null",
	"int",
	"Class",
	"array",
	"float",
	"func",
	"char",
	"String",
	"Boolean"
};

Paramlist alc_param(char *name, Typeptr type) {
	Paramlist params = (Paramlist) malloc(sizeof(struct param));
	params->name = name;
	params->type = type;
	params->next = NULL;

	return params;
}

void print_param_list(Paramlist paramlist) {
	Paramlist current = paramlist;

	while(current) {
		printf("Param Name: %s, Type: %s\n", current->name, getTypeName(current->type->basetype));
		current = current->next;
	}
}

int count_param_list(Paramlist paramlist) {
	Paramlist current = paramlist;
	int count = 0;

	while(current) {
		++count;
		current = current->next;
	}

	return count;
}

// Adapted from: https://www.educative.io/courses/coderust-hacking-the-coding-interview/lq2j
Paramlist reverse_param_list(Paramlist head_params) {
	Paramlist reversed_params = NULL, curr_param = head_params->next, temp;

	// There is only one or no nodes, just return the Paramlist
	if (head_params == NULL || head_params->next == NULL)
		return head_params;

	// Begin reversing the parameters. Assign first node to new list.
	reversed_params = head_params;
	reversed_params->next = NULL;

	// Reverse until there are none left
	while(curr_param) {
		// Insert current parameter in at head of reversed list with temp
		temp = curr_param;
		curr_param = curr_param->next;

		temp->next = reversed_params;
		reversed_params = temp;
	}

	return reversed_params;
}

Paramlist arg_list_to_params(struct tree *head) {
	Paramlist parameters = NULL, curr_param = NULL;
	Typeptr type = NULL;
	char *name = NULL;
	struct tree *curr_node = head;

	if(head == NULL)
		return NULL;

	// The node passed is not an ArgList!
	if (head->prodrule != PR_ARG_LIST) {
		if (head->prodrule == TOKEN) {
			name = curr_node->leaf->text;
			type = get_type(curr_node);
			parameters = alc_param(name, type);
			return parameters;
		}
		return NULL;
	}

	// Allocate first param and start of list
	name = curr_node->kids[1]->leaf->text;
	type = get_type(curr_node->kids[1]);
	parameters = alc_param(name, type);
	//printf("TESTING: %s\n\n", parameters->name);

	// Pointer to the last inserted param
	curr_param = parameters;

	// Advance to next arg in list
	curr_node = curr_node->kids[0];

	// Loop till the end of the list, link each to param list
	while(curr_node->prodrule == PR_ARG_LIST) {
		name = curr_node->kids[1]->leaf->text;
		type = get_type(curr_node->kids[1]);
		curr_param->next = alc_param(name, type);
		curr_param = curr_param->next;
		curr_node = curr_node->kids[0];
	}

	// Add the last node and reverse param list
	name = curr_node->leaf->text;
	type = get_type(curr_node);
	curr_param->next = alc_param(name, type);

	parameters = reverse_param_list(parameters);
	//print_param_list(parameters);

	return parameters;
}

Paramlist formal_args_to_list(struct tree *current_node) {
	Typeptr param_type = NULL;
	char *param_name = NULL;
	Paramlist func_params = NULL, current_param = NULL;
	int nparams = 0;

	if (current_node == NULL) {
		return NULL;
	}

	if(current_node->prodrule == PR_FORMAL_PARM) {
		param_name = current_node->kids[1]->leaf->text;
		param_type = string_to_type(current_node->kids[0]->leaf->text);
		//printf("Type: %s, Var: %s\n", param_name, current_node->kids[0]->leaf->text);
		nparams++;
		func_params = alc_param(param_name, param_type);
		current_param = func_params;
		return func_params;

	} else {
		param_name = current_node->kids[1]->kids[1]->leaf->text;
		param_type = string_to_type(current_node->kids[1]->kids[0]->leaf->text);
		//printf("Type: %s, Var: %s\n", param_name, current_node->kids[1]->kids[0]->leaf->text);
		nparams++;

		current_node = current_node->kids[0];
	}

	func_params = alc_param(param_name, param_type);
	current_param = func_params;

	while (current_node->prodrule == PR_FORMAL_PARM_LIST) {
		param_name = current_node->kids[1]->kids[1]->leaf->text;
		param_type = string_to_type(current_node->kids[1]->kids[0]->leaf->text);
		printf("MADE IT HERE Type: %s, Var: %s\n", param_name, current_node->kids[1]->kids[0]->leaf->text);
		nparams++;
		current_param->next = alc_param(param_name, param_type);
		current_param = current_param->next;

		current_node = current_node->kids[0];
	}

	if (nparams > 0) {
		printf("NUM OF PARAMS LOL: %d\n\n\n", nparams);
		param_name = current_node->kids[1]->leaf->text;
		param_type = string_to_type(current_node->kids[0]->leaf->text);
		current_param->next = alc_param(param_name, param_type);
		current_param = current_param->next;
		//printf("Type: %s, Var: %s\n", param_name, current_node->kids[0]->leaf->text);
		nparams++;
	}

	func_params = reverse_param_list(func_params);
	//print_param_list(func_params);

	return func_params;
}

Typeptr alc_type(int base) {

	Typeptr rv = (Typeptr) malloc(sizeof(struct typeinfo));
	rv->basetype = base;

	return rv;
}

Typeptr alc_func_type(struct tree *n) {
	Typeptr tp = alc_type(FUNC_TYPE);

	struct tree *current_node = n->kids[3]->kids[1];
	Paramlist func_params = NULL;
	int nparams = 0;

	func_params = formal_args_to_list(current_node);
	nparams = count_param_list(func_params);
	//printf("TEST: nparams = %d\n", nparams);

	//printf("INSIDE PARAMS: %d\n", nparams);
	//print_param_list(func_params);

	tp->u.f.name = n->kids[3]->kids[0]->leaf->text;
	tp->u.f.nparams = nparams;

	if (n->kids[2]->prodrule == PR_TYPE_ARRAY) {
		tp->u.f.returntype = string_to_type(n->kids[2]->kids[0]->leaf->text);
	} else {
		tp->u.f.returntype = string_to_type(n->kids[2]->leaf->text);
	}

	tp->u.f.parameters = func_params;

	return tp;
}

Typeptr alc_class_type(struct tree *n) {
	Typeptr tp = alc_type(CLASS_TYPE);
	tp->u.c.label = n->kids[2]->leaf->text;

	return tp;
}

const char *getTypeName(int basetype) {
	return typenames[basetype - FIRST_TYPE];
}

Typeptr string_to_type(char *str) {
	Typeptr type;
	if (!strcmp(str,"int") || !strcmp(str,"short") || !strcmp(str,"long")) {
		//printf("FOUND INT!\n");
		type = alc_type(INT_TYPE);
	} else if (!strcmp(str, "char")) {
		//printf("FOUND CHAR!\n");
		type = alc_type(CHAR_TYPE);
	} else if (!strcmp(str, "boolean")) {
		//printf("FOUND BOOL!\n");
		type = alc_type(BOOLEAN_TYPE);
	} else if (!strcmp(str, "float") || !strcmp(str, "double")) {
		//printf("FOUND FLOAT!\n");
		type = alc_type(FLOAT_TYPE);
	} else if (!strcmp(str, "String")) {
		//printf("FOUND STRING!\n");
		type = alc_type(STRING_TYPE);
	} else if (!strcmp(str, "void")) {
		type = alc_type(VOID_TYPE);
	} else {
		return NULL;
	}

	return type;
}

Typeptr get_type(struct tree *n) {
	Typeptr type;
	SymbolTableEntry ste;

	if (n->prodrule == TOKEN) {
		ste = lookup_st(current, n->leaf->text);

		if (ste) { return ste->type; }

		switch(n->leaf->category) {
			case STRING_LITERAL:
				type = alc_type(STRING_TYPE);
				break;
			case INTEGER_LITERAL:
				type = alc_type(INT_TYPE);
				break;
			case FLOAT_LITERAL:
				type = alc_type(FLOAT_TYPE);
				break;
			case CHARACTER_LITERAL:
				type = alc_type(CHAR_TYPE);
				break;
			case BOOLEAN_LITERAL:
				type = alc_type(BOOLEAN_TYPE);
				break;
			default:
				//semantic_error("I don't know what type this is.", n);
				//printf("made it here\n");
				return NULL;
		}
	} else {
		if (n->nkids != 0 && n->prodrule == PR_TYPE_ARRAY) {
			type = get_type(n->kids[0]);
		}
		type = n->type;
	}

	return type;
}

// int type_promotion(Typeptr t1, Typeptr t2) {
// 	int type_int;
// 	if (t1->basetype == t2->basetype)
// 		type_int = t1->basetype;
//
//
// 	return type_int;
// }
