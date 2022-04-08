/*
 * symt.c
 */

/*
 * The functions in this file maintain a hash table mapping strings to
 *   symbol table entries.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symt.h"
#include "error.h"
#include "defs.h"

void dovariabledeclarator(struct tree *n);
//void semanticerror(char *s, struct tree *n);
//extern int errors;

SymbolTable globals;
SymbolTable current;

char *alloc(int n);		    /* calloc + check for NULL */


/*
 * new_st - construct symbol (hash) table.
 *  Allocate space first for the structure, then
 *  for the array of buckets.
 */
SymbolTable new_st(char *name, int nbuckets)
   {
   //int h;
   SymbolTable rv;
   rv = (SymbolTable) alloc(sizeof(struct sym_table));
   rv->tbl = (struct sym_entry **)
      alloc((unsigned int)(nbuckets * sizeof(struct sym_entry *)));
   rv->nBuckets = nbuckets;
   rv->nEntries = 0;
   rv->parent = NULL;
   rv->name = name;
   return rv;
   }

/*
 * delete_st - destruct symbol table.
 */
void delete_st(SymbolTable st)
   {
   SymbolTableEntry se, se1;
   int h;

   for (h = 0; h < st->nBuckets; ++h)
      for (se = st->tbl[h]; se != NULL; se = se1) {
         se1 = se->next;
         free((char *)se->s); /* strings in the table were all strdup'ed. */
         free((char *)se);
         }
   free((char *)st->tbl);
   free((char *)st);
   }

/*
 * Compute hash value.
 */
int hash(SymbolTable st, char *s)
{
   register int h = 0;
   register char c;
   while ((c = *s++)) {
      h += c & 0377;
      h *= 37;
      }
   if (h < 0) h = -h;
   return h % st->nBuckets;
}

/*
 * Insert a symbol into a symbol table.
 */
//int insert_sym(SymbolTable st, char *s, Typeptr t)
int insert_sym(SymbolTable st, char *s, Typeptr t)
{
	//register int i;
	int h;
	struct sym_entry *se;
	//int l;

	h = hash(st, s);
	for (se = st->tbl[h]; se != NULL; se = se->next)
		if (!strcmp(s, se->s)) {
			/* A copy of the string is already in the table. */
			return 0;
		}

	/*
	* The string is not in the table. Add the copy from the
	*  buffer to the table.
	*/
	se = (SymbolTableEntry)alloc((unsigned int) sizeof (struct sym_entry));
	se->next = st->tbl[h];
	se->table = st;
	st->tbl[h] = se;
	se->s = strdup(s);
	//se->type = type;
	se->type= t;
	st->nEntries++;
	return 1;
}

/*
 * lookup_st - search the symbol table for a given symbol, return its entry.
 */
SymbolTableEntry lookup_st(SymbolTable st, char *s)
{
	//register int i;
	int h;
	SymbolTable sst;
	SymbolTableEntry se;

	sst = st;

	while (sst) {
		h = hash(sst, s);
		for (se = sst->tbl[h]; se != NULL; se = se->next)
			if (!strcmp(s, se->s)) {
				/* Return a pointer to the symbol table entry. */
				return se;
			}
		sst = sst->parent;
	}
	return NULL;
}

char * alloc(int n)
{
   char *a = calloc(n, sizeof(char));
   if (a == NULL) {
      fprintf(stderr, "alloc(%d): out of memory\n", (int)n);
      exit(-1);
      }
   return a;
}

int enter_newscope(char *s, Typeptr tp)
{
	/* allocate a new symbol table */
	/* insert s into current symbol table */
	/* attach new symbol table to s's symbol table in the current symbol table*/
	/* push new symbol on the stack, making it the current symbol table */
	SymbolTable nst = new_st(s, 20);

	if (tp->basetype == CLASS_TYPE) {
		tp->u.c.st = nst;
	} else {
		tp->u.f.st = nst;
	}

	if (!insert_sym(current, s, tp)) {
		return 0;
	}

	pushscope(nst);
	return 1;
}

void populate_symboltables(struct tree * n)
{
   int i;
   if (n == NULL) return;

   /* pre-order activity */
   switch (n->prodrule) {
        case PR_METHOD_DECLARATOR/* whatever production rule(s) enter a function scope */ : {
			func_declaration(n);
            break;
        }

        case PR_CLASS_DECL: {
			class_declaration(n);
            break;
        }

		case PR_INSTANTIATION_EXPR: {
			semantic_error("No class instantiation!", n->kids[0]);
		}

		case PR_FIELD_DECL:
		case PR_LOCAL_VAR_DECL:
		case PR_FIELD_DECL_ASSIGN:
		case PR_FORMAL_PARM:
		case PR_ASSIGNMENT_DECL:
		case PR_FIELD_DECL_MOD: {
			dovariabledeclarator(n);
			break;
		}

		case PR_QUALIFIED_NAME: {
			check_qualified_name(current, n);
			break;
		}

		case TOKEN: {
			if (n->leaf->category == IDENTIFIER) {
				if(lookup_st(current, n->leaf->text) == NULL) {
					semantic_error("There is no matching symbol in the symbol table!", n);
				}
			}
			break;
        }

   }

   /* visit children */
	switch (n->prodrule) {
		case PR_QUALIFIED_NAME:
			break;
		default:
			for (i=0; i<n->nkids; i++)
				populate_symboltables(n->kids[i]);
		   	break;
	}

	/* post-order activity */
	switch (n->prodrule) {
		case PR_METHOD_DECL:
		case PR_CLASS_DECL:
			popscope();
			break;
		default:
			type_checker(n);
			break;
	}

}

void type_checker(struct tree *n) {
//struct tree *node;
Typeptr type, other_type;
int int_to_float = 0;
//char *s;

switch (n->prodrule) {
	case PR_METHODCALL_P:
		// node = n->kids[0];
		// if (node->prodrule != PR_QUALIFIED_NAME) {
		// 	printf("Function PR_METHODCALL_P: %s\n", n->kids[0]->leaf->text);
		// } else {
		// 	printf("Builtin PR_METHODCALL_P: ");
		// 	while (node->kids[1]->prodrule == PR_QUALIFIED_NAME) {
		// 		printf("%s.", node->kids[0]->leaf->text);
		// 		node = node->kids[1];
		// 	}
		// 	printf("%s.%s\n", node->kids[0]->leaf->text, node->kids[1]->leaf->text);
		// }
		//
		// node = n->kids[1];
		// if (node != NULL)
		// 	printf("Checking this symbol's arguments: %s\n", node->symbolname);
		// while()
	break;

	case PR_UNARY_EXPR_NEG:
		type = get_type(n->kids[1]);

		switch (type->basetype) {
			case CHAR_TYPE:
			case INT_TYPE:
			case FLOAT_TYPE:
				break;
			default:
				semantic_error("This is not a proper type for a unary negation.", n->kids[1]);
		}

		if (type->basetype == FLOAT_TYPE) {
			n->type = alc_type(FLOAT_TYPE);
		} else {
			n->type = alc_type(INT_TYPE);
		}

	break;

	//very similar to PR_UNARY_EXPR_NEG
	//Come back later to shorten
	case PR_UNARY_EXPR_NOT:
		type = get_type(n->kids[1]);

		switch (type->basetype) {
			case BOOLEAN_TYPE:
				break;
			default:
				semantic_error("This is not a proper type for a unary not.", n->kids[1]);
		}

		n->type = alc_type(BOOLEAN_TYPE);
	break;

	case PR_MUL_EXPR_MULT:
	case PR_MUL_EXPR_DIV:
	case PR_MUL_EXPR_MOD:
		for (int i = 0; i < 2; i++) {
			type = get_type(n->kids[i]);

			//just a print to see expression values
			// if (n->kids[i]->prodrule == TOKEN) {
			// 	printf("Lineno: %d   Token: %s\n",
			// 	n->kids[i]->leaf->lineno, n->kids[i]->leaf->text);
			// } else {
			//
			// 	printf("The type is: %s\n", getTypeName(type->basetype));
			// }

			switch (type->basetype) {
				case FLOAT_TYPE:
					int_to_float=1;
				case INT_TYPE:
				case CHAR_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for a multiplication expression.", n->kids[i]);
			}
		}

		if (int_to_float) {
			n->type = alc_type(FLOAT_TYPE);
		} else {
			n->type = alc_type(INT_TYPE);
		}
	break;

	//This is the same case as PR_MUL_EXPR
	////Come back later to shorten
	case PR_ADD_EXPR_ADD:
	case PR_ADD_EXPR_SUB:
		for (int i = 0; i < 2; i++) {
			type = get_type(n->kids[i]);

			switch (type->basetype) {
				case FLOAT_TYPE:
					int_to_float=1;
				case INT_TYPE:
				case CHAR_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for a multiplication expression.", n->kids[i]);
			}
		}

		if (int_to_float) {
			n->type = alc_type(FLOAT_TYPE);
		} else {
			n->type = alc_type(INT_TYPE);
		}
		break;

	case PR_REL_EXPR:
		for (int i = 0; i < 3; i+=2) {
			type = get_type(n->kids[i]);

			switch (type->basetype) {
				case FLOAT_TYPE:
				case INT_TYPE:
				case CHAR_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for a relational expression.", n->kids[i]);
			}
		}
		n->type = alc_type(BOOLEAN_TYPE);

	case PR_EQ_EXPR:
	case PR_EQ_EXPR_NOT:
		type = get_type(n->kids[0]);
		other_type = get_type(n->kids[2]);

		if (type != other_type) {
			switch (type->basetype) {
				case FLOAT_TYPE:
				case INT_TYPE:
				case CHAR_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for a equality expression.", n->kids[0]);
			}
			switch (other_type->basetype) {
				case FLOAT_TYPE:
				case INT_TYPE:
				case CHAR_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for an equality expression.", n->kids[2]);
			}
		}

		n->type = alc_type(BOOLEAN_TYPE);
		break;

		case PR_COND_OR_EXPR:
		case PR_COND_AND_EXPR:
			for (int i = 0; i < 2; i++) {
				type = get_type(n->kids[i]);

				if (type->basetype != BOOLEAN_TYPE) {
					semantic_error("This is not a boolean type for a Conditional Expression.", n->kids[i]);
				}
			}
			n->type = alc_type(BOOLEAN_TYPE);
			break;

		case PR_ASSIGNMENT_UNARY:
			type = get_type(n->kids[0]);

			switch (type->basetype) {
				case CHAR_TYPE:
				case INT_TYPE:
				case FLOAT_TYPE:
					break;
				default:
					semantic_error("This is not a proper type for a unary negation.", n->kids[0]);
			}
			n->type = type;
			break;

		case PR_ASSIGNMENT:
			type = get_type(n->kids[0]);
			other_type = get_type(n->kids[2]);

			if (type != other_type) {
				switch (type->basetype) {
					case FLOAT_TYPE:
						switch (other_type->basetype) {
							case FLOAT_TYPE:
							case INT_TYPE:
							case CHAR_TYPE:
								other_type = alc_type(FLOAT_TYPE);
								break;
							default:
								semantic_error("Mismatched type for assignment",
									n->kids[0]);
						}
						break;
					case INT_TYPE:
						switch (other_type->basetype) {
							case FLOAT_TYPE:
								semantic_error("No type demotion.", n->kids[0]);
							case INT_TYPE:
							case CHAR_TYPE:
								other_type = alc_type(INT_TYPE);
								break;
							default:
								semantic_error("Mismatched type for assignment",
								n->kids[0]);
						}
						break;
					case CHAR_TYPE:
						switch (other_type->basetype) {
							case FLOAT_TYPE:
							case INT_TYPE:
								semantic_error("No type demotion.", n->kids[0]);
								break;
							default:
								semantic_error("Mismatched type for assignment",
									n->kids[0]);
						}
					default:
						semantic_error("This is not a proper type for an assignment expression.", n->kids[0]);
				}
			}
			break;

		case PR_ASSIGNMENT_DECL:
			break;
}
}

void printsyms(struct tree * n)
{
    int i;
    if (n == NULL) return;
    /* pre-order activity */
    switch (n->prodrule) {
        case PR_METHOD_DECLARATOR:
            printf("Method Decl: %s\n", n->kids[0]->leaf->text);
            break;
        case PR_VAR_DECLS:
            printf("Var Decl: %s\n", n->kids[0]->leaf->text);
            break;
        case PR_VAR_DECLS_LIST:
            printf("Var Decl: %s\n", n->kids[1]->leaf->text);
            break;
		case PR_CLASS_DECL:
			printf("Class Decl: %s\n", n->kids[2]->leaf->text);
			break;
        case PR_ASSIGNMENT_DECL:
            printf("Assign Decl: %s\n", n->kids[1]->leaf->text);
			break;
		case PR_FIELD_DECL_ASSIGN:
			printf("Field Decl: %s\n", n->kids[1]->leaf->text);
		case PR_FORMAL_PARM:
			printf("Formal Param: %s\n", n->kids[1]->leaf->text);
    }


   /* visit children */
   for (i=0; i<n->nkids; i++)
        printsyms(n->kids[i]);
}

void dovariabledeclarator(struct tree * n)
{
	//printf("This is the check for type: %s\n", n->kids[0]->leaf->text);
	char *type = n->kids[0]->leaf->text;
	Typeptr var;

	if (!strcmp(type,"int")) {
		//printf("FOUND INT!\n");
		var = alc_type(INT_TYPE);
	} else if (!strcmp(type, "char")) {
		//printf("FOUND CHAR!\n");
		var = alc_type(CHAR_TYPE);
	} else if (!strcmp(type, "boolean")) {
		//printf("FOUND BOOL!\n");
		var = alc_type(BOOLEAN_TYPE);
	} else if (!strcmp(type, "float") || !strcmp(type, "double")) {
		//printf("FOUND FLOAT!\n");
		var = alc_type(FLOAT_TYPE);
	} else if (!strcmp(type, "String")) {
		//printf("FOUND STRING!\n");
		var = alc_type(STRING_TYPE);
	}
	// else {
	// 	semantic_error("There should be no class instantiation in j0.1!", n->kids[0]);
		// if (!lookup_st(current, n->kids[0]->leaf->text)) {
		// 	semantic_error("There is no class by this name.", n->kids[0]);
		// }
		// SymbolTable nst = new_st(n->kids[1]->leaf->text, 20);
		// var = alc_class_type(nst, n->kids[0]->leaf->text);
	// }

	switch (n->prodrule) {
		case PR_FIELD_DECL:
		case PR_FIELD_DECL_MOD:
		case PR_LOCAL_VAR_DECL:
			if (!insert_sym(current, n->kids[1]->kids[0]->leaf->text , var)) {
				semantic_error("There exists a variable by the same name.", n->kids[1]->kids[0]);
			}
            break;
		case PR_FIELD_DECL_ASSIGN:
		case PR_FORMAL_PARM:
		case PR_ASSIGNMENT_DECL:
			//printf("Inserting new var %s\n", n->kids[1]->leaf->text);
			if (!insert_sym(current, n->kids[1]->leaf->text, var)) {
				semantic_error("There exists a variable by the same name.", n->kids[1]);
			}

			break;
	}


}

void class_declaration(struct tree *n) {
	Typeptr type = alc_class_type(n);
	if (!enter_newscope(n->kids[2]->leaf->text, type)) {
		semantic_error("There exists a class by the same name.", n->kids[2]);
	}
}

void func_declaration(struct tree *n) {
	Typeptr type = alc_func_type(n);
	if (!enter_newscope(n->kids[0]->leaf->text, type)) {
		semantic_error("There exists a function by the same name.", n->kids[0]);
	}
}

void printsymbols(SymbolTable st, int level)
{
	int i, j;
   	SymbolTableEntry ste;
	if (st == NULL) return;
	for (i=0;i<st->nBuckets;i++) {
		for (ste = st->tbl[i]; ste; ste=ste->next) {
			//for (j=0; j < level; j++) printf("  |  ");
			for (j=0; j < level; j++) printf("\t");
			//printf("%s - level = %d\n", ste->s, level);
			if (ste->type) {
				switch (ste->type->basetype) {
					case NULL_TYPE:
						printf("NULL ");
						break;
					case INT_TYPE:
						printf("int ");
						break;
					case CLASS_TYPE:
						printf("Class ");
						break;
					case ARRAY_TYPE:
						printf("Array ");
						break;
					case FLOAT_TYPE:
						printf("double ");
						break;
					case FUNC_TYPE:
						printf("Func ");
						break;
					case BOOLEAN_TYPE:
						printf("Bool ");
						break;
					case CHAR_TYPE:
						printf("char ");
						break;
					case STRING_TYPE:
						printf("String ");
				}
			}

			printf("%s\n", ste->s);

			if (!ste->type) continue;
			switch (ste->type->basetype) {
				case CLASS_TYPE:
					//for (j=0; j < level+1; j++) (j == level - 1 ? printf("  |  ") : printf("  |"));
					for (j=0; j < level+1; j++) printf("\t");
					printf("--- Symbol Table for Class: %s ---\n", ste->s);

					printsymbols(ste->type->u.c.st, level + 1);

					//for (j=0; j < level+1; j++) (j != level ? printf("|  ") : printf("└"));
					for (j=0; j < level+1; j++) printf("\t");
					printf("---- Ending Symbol Table ----\n");
					break;
				case FUNC_TYPE:
					//for (j=0; j < level+1; j++) (j == level - 1 ? printf("  |  ") : printf("  |"));
					for (j=0; j < level+1; j++) printf("\t");
					printf("--- Symbol Table for Method: %s ---\n", ste->s);

					printsymbols(ste->type->u.f.st, level + 1);

					//for (j=0; j < level+1; j++) (j != level ? printf("|  ") : printf("└ "));
					for (j=0; j < level+1; j++) printf("\t");
					printf("---- Ending Symbol Table ----\n");
					break;
			}
      	}
   	}
}

void create_builtin_packages(SymbolTable global) {
	//SymbolTable System_st, out_st;

	enter_newscope("System", alc_type(CLASS_TYPE));
		enter_newscope("out", alc_type(CLASS_TYPE));
			enter_newscope("println", alc_type(FUNC_TYPE));
			popscope();

			enter_newscope("print", alc_type(FUNC_TYPE));
			popscope();
		popscope();

		enter_newscope("in", alc_type(CLASS_TYPE));
			enter_newscope("read", alc_type(FUNC_TYPE));
			popscope();
		popscope();
	popscope();

	enter_newscope("String", alc_type(CLASS_TYPE));

		enter_newscope("charAt", alc_type(FUNC_TYPE));
		popscope();

		enter_newscope("equals", alc_type(FUNC_TYPE));
		popscope();

		enter_newscope("compareTo", alc_type(FUNC_TYPE));
		popscope();

		enter_newscope("length", alc_type(FUNC_TYPE));
		popscope();

		enter_newscope("toString", alc_type(FUNC_TYPE));
		popscope();

	popscope();

	enter_newscope("InputStream", alc_type(CLASS_TYPE));
		enter_newscope("read", alc_type(FUNC_TYPE));
		popscope();
	popscope();
}

void check_qualified_name(SymbolTable current, struct tree *n) {
	SymbolTable st = current;
	SymbolTableEntry ste;
	struct tree *node = n;

	while (node->prodrule == PR_QUALIFIED_NAME) {
		ste = lookup_st(st, node->kids[0]->leaf->text);
		if (ste == NULL) {
			semantic_error("This is not a valid built-in package!", node->kids[0]);
		}
		//printf("QualifiedName: %s\n", node->kids[0]->leaf->text);
		if (ste->type->basetype == CLASS_TYPE) {
			st = ste->type->u.c.st;
		}
		//printf("Changing symboltable to %s\n", st->name);

		if(node->kids[1]->prodrule != PR_QUALIFIED_NAME) {
			ste = lookup_st(st, node->kids[1]->leaf->text);
			if (ste == NULL) {
				semantic_error("This is not a valid built-in subpackage!", node->kids[1]);
			}
			//printf("QualifiedName: %s\n", node->kids[1]->leaf->text);
		}

		node = node->kids[1];
	}
}

/*
void semanticerror(char *s, struct tree *n)
{
   while (n && (n->nkids > 0)) n=n->kids[0];
   if (n) {
     fprintf(stderr, "%s:%d: ", n->leaf->filename, n->leaf->lineno);
   }

  fprintf(stderr, "%s", s);
  if (n && n->prodrule == PR_IDENT) fprintf(stderr, " %s", n->leaf->text);
  fprintf(stderr, "\n");
  errors++;
}
*/
