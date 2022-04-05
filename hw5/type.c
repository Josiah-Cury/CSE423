#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

Typeptr alc_type(int base) {

	Typeptr rv = (Typeptr) malloc(sizeof(struct typeinfo));
	rv->basetype = base;

	return rv;
}

Typeptr alc_func_type(SymbolTable st, char *s) {
	Typeptr tp = alc_type(FUNC_TYPE);
	tp->u.f.st = st;
	tp->u.f.name = s;

	return tp;
}

Typeptr alc_class_type(SymbolTable st, char *s) {
	Typeptr tp = alc_type(CLASS_TYPE);
	tp->u.c.st = st;
	tp->u.c.label = s;

	return tp;
}
