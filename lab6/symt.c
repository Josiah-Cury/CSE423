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
void semanticerror(char *s, struct tree *n);
extern int errors;

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
   rv->parent == NULL;
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
         /*
          * A copy of the string is already in the table.
          */

		 semantic_error(strcat(s, " symbol is already in table!\n"));
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

	h = hash(sst, s);
	for (se = sst->tbl[h]; se != NULL; se = se->next)
		if (!strcmp(s, se->s)) {
			/*
			*  Return a pointer to the symbol table entry.
			*/
			return se;
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

void enter_newscope(char *s, int type)
{
   /* allocate a new symbol table */
   /* insert s into current symbol table */
   /* attach new symbol table to s's symbol table in the current symbol table*/
   /* push new symbol on the stack, making it the current symbol table */
   SymbolTable nst = new_st(s, 20);
   Typeptr tp;
   if (type == CLASS_TYPE) {
	   tp = alc_class_type(nst);
   } else {
	   tp = alc_func_type(nst);
   }
   insert_sym(current, s, tp);

   pushscope(nst);
}

void populate_symboltables(struct tree * n)
{
   int i;
   if (n == NULL) return;
   /* pre-order activity */
   switch (n->prodrule) {
        case PR_METHOD_DECLARATOR/* whatever production rule(s) enter a function scope */ : {
            enter_newscope(n->kids[0]->leaf->text, FUNC_TYPE);
            break;
        }
        case PR_CLASS_DECL: {
            enter_newscope(n->kids[2]->leaf->text, CLASS_TYPE);
            break;
        }
        case PR_VAR_DECLS/* whatever production rule(s) designate a variable declaration*/: {
            /* figure out which kid is a "list" of variables */
            /* walk through the subtree that is the list of variables */
            /* for each variable, insert it into the current symbol table*/
            //printf("Correctly inserted!\n");
            Typeptr var = alc_type(NULL_TYPE);
            insert_sym(current, n->kids[0]->leaf->text, NULL);
            break;
        }
        case PR_VAR_DECLS_LIST:
		case PR_FIELD_DECL_ASSIGN:
		case PR_ASSIGNMENT_DECL:{
            insert_sym(current, n->kids[1]->leaf->text, NULL);
            break;
        }
        case 1/* whatever leaf denotes a variable name */: {
            SymbolTableEntry ste = NULL;
            SymbolTable st = current;
            /* check if the symbol is already defined in current scope */
            /*   if it is, report a redeclaration error */
            /*   if it is not, insert it into the current symbol table */
        }
   }
	if (n->prodrule == PR_ASSIGNMENT || n->prodrule == PR_ASSIGNMENT_UNARY) {
		if(lookup_st(current, n->kids[0]->leaf->text) != NULL) semantic_error("There is no symbol in the symbol table!");
		/* code */
	}
   /* visit children */
   for (i=0; i<n->nkids; i++)
      populate_symboltables(n->kids[i]);

   // /* post-order activity */
   // switch (n->prodrule) {
   //
   //     case /* we are back out to a node where we entered a subscope */:
   //       popscope();
   //       break;
   // }
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
    }


   /* visit children */
   for (i=0; i<n->nkids; i++)
        printsyms(n->kids[i]);
}

// void dovariabledeclarator(struct tree * n)
// {
//   /* in future look for type information (e.g. array, pointer) modifiers */
//   /* get variable ident */
//
//   insert_sym(current, ident);
// }

void printsymbols(SymbolTable st, int level)
{
   	int i, j;
   	SymbolTableEntry ste;
	if (st == NULL) return;
	for (i=0;i<st->nBuckets;i++) {
		for (ste = st->tbl[i]; ste; ste=ste->next) {
			for (j=0; j < level; j++) printf("  ");
			//printf("%s - level = %d\n", ste->s, level);
			printf("%s\n", ste->s);
			// if(!strcmp(ste->s, "o")) {
			// 	printf("This is the parent symbol table of o: %s\n", ste->table->name);
			// }

			if (!ste->type) continue;
			if (ste->type->st != NULL) {
				for (j=0; j < level+1; j++) printf("  ");
				printf("--- symbol table for: %s ---\n", ste->s);

				printsymbols(ste->type->st, level + 1);

				for (j=0; j < level+1; j++) printf("  ");
				printf("----------------------------\n");
			}
      	}
   	}
}

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
