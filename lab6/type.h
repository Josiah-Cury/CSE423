#ifndef TYPE_H
#define TYPE_H

#include "defs.h"

typedef struct sym_table *SymbolTable;

#define FIRST_TYPE   1000000

#define NULL_TYPE    1000000
#define INT_TYPE     1000001
#define CLASS_TYPE  1000002
#define ARRAY_TYPE   1000003
#define FLOAT_TYPE   1000004
#define FUNC_TYPE    1000005
#define CHAR_TYPE    1000006
#define STRING_TYPE	 1000007
#define BOOLEAN_TYPE 1000008

#define LAST_TYPE    1000008

typedef struct typeinfo {
	int basetype;
	struct sym_table *st;
} *Typeptr;

Typeptr alc_type(int base);
Typeptr alc_func_type(SymbolTable st);
Typeptr alc_class_type(SymbolTable st);

#endif
