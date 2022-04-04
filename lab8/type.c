#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

char *typename[] =
   {"null", "int", "class", "array", "float", "func", "char", "string", "boolean"};

Typeptr alc_type(int base) {
	//struct typeinfo t_info = {NULL_TYPE, NULL};


	Typeptr tp = (Typeptr) malloc(sizeof(struct typeinfo));
	// switch (base) {
	// 	case NULL_TYPE:
	// 		tp = (Typeptr){.basetype = NULL_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case INT_TYPE:
	// 		tp = (Typeptr){.basetype = INT_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case ARRAY_TYPE:
	// 		tp = (Typeptr){.basetype = ARRAY_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case FLOAT_TYPE:
	// 		tp = (Typeptr){.basetype = FLOAT_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case CHAR_TYPE:
	// 		tp = (Typeptr){.basetype = CHAR_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case STRING_TYPE:
	// 		tp = (Typeptr){.basetype = STRING_TYPE, .subtab = NULL};
	// 		return tp;
	// 	case BOOLEAN_TYPE:
	// 		tp = (Typeptr){.basetype = BOOLEAN_TYPE, .subtab = NULL};
	// 		return tp;
	// }
	tp->basetype = base;
	return tp;
}

Typeptr alc_func_type(SymbolTable st) {
	Typeptr tp = alc_type(FUNC_TYPE);
	tp->u.f.st = st;

	return tp;
}

Typeptr alc_class_type(SymbolTable st) {
	Typeptr tp = alc_type(CLASS_TYPE);
	tp->u.c.st = st;

	return tp;
}

char *get_typename(Typeptr t) {
	if (t->basetype < FIRST_TYPE || t->basetype > LAST_TYPE) {
		return "INVALID";
	}
	return typename[t->basetype-1000000];
}
