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

int enter_newscope(char *s, int type)
{
	/* allocate a new symbol table */
	/* insert s into current symbol table */
	/* attach new symbol table to s's symbol table in the current symbol table*/
	/* push new symbol on the stack, making it the current symbol table */
	SymbolTable nst = new_st(s, 20);
	Typeptr tp;

	if (type == CLASS_TYPE) {
		tp = alc_class_type(nst, s);
	} else {
		tp = alc_func_type(nst, s);
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
			if (!enter_newscope(n->kids[0]->leaf->text, FUNC_TYPE)) {
				semantic_error("There exists a function by the same name.", n->kids[0]);
			}
            break;
        }
        case PR_CLASS_DECL: {
			if (!enter_newscope(n->kids[2]->leaf->text, CLASS_TYPE)) {
				semantic_error("There exists a class by the same name.", n->kids[2]);
			}
            break;
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
	} else {
		semantic_error("There should be no class instantiation in j0.1!", n->kids[0]);
		if (!lookup_st(current, n->kids[0]->leaf->text)) {
			semantic_error("There is no class by this name.", n->kids[0]);
		}
		SymbolTable nst = new_st(n->kids[1]->leaf->text, 20);
		var = alc_class_type(nst, n->kids[0]->leaf->text);
	}

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

	enter_newscope("System", CLASS_TYPE);
		enter_newscope("out", CLASS_TYPE);
			enter_newscope("println", FUNC_TYPE);
			popscope();

			enter_newscope("print", FUNC_TYPE);
			popscope();
		popscope();

		enter_newscope("in", CLASS_TYPE);
			enter_newscope("read", FUNC_TYPE);
			popscope();
		popscope();
	popscope();

	enter_newscope("String", CLASS_TYPE);

		enter_newscope("charAt", FUNC_TYPE);
		popscope();

		enter_newscope("equals", FUNC_TYPE);
		popscope();

		enter_newscope("compareTo", FUNC_TYPE);
		popscope();

		enter_newscope("length", FUNC_TYPE);
		popscope();

		enter_newscope("toString", FUNC_TYPE);
		popscope();

	popscope();

	enter_newscope("InputStream", CLASS_TYPE);
		enter_newscope("read", FUNC_TYPE);
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
