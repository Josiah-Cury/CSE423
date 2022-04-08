#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

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
