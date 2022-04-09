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

Typeptr alc_type(int base) {

	Typeptr rv = (Typeptr) malloc(sizeof(struct typeinfo));
	rv->basetype = base;

	return rv;
}

Typeptr alc_func_type(struct tree *n) {
	Typeptr tp = alc_type(FUNC_TYPE);
	tp->u.f.name = n->kids[0]->leaf->text;

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
		if (ste) {
			return ste->type;
		}

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
