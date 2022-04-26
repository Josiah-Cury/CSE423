/* SYMTAB_H */

/*
 * If I ever finish this, it will be a stripped down version of symtab * that
 * has removed the string buffery stuff.
 */
#ifndef SYMT_H
#define SYMT_H

//#include "tree.h"
#include "type.h"
typedef struct param *Paramlist;
typedef struct typeinfo *Typeptr;

struct string_list {
	struct tree *string_node;
	struct string_list *next;
};

typedef struct sym_table {
 	int nBuckets; /* # of buckets */
 	int nEntries; /* # of symbols in the table */
	int byte_count;
	char *name;
 	struct sym_table *parent; /* enclosing scope, superclass etc. */
 	struct sym_entry **tbl; /* more per-scope/per-symbol-table attributes go here */
} *SymbolTable;

/*
 * Entry in symbol table.
 */
typedef struct sym_entry {
	SymbolTable table; /* what symbol table do we belong to*/
	struct typeinfo *type;
	struct addr *address;
	char *s; /* string */
	/* more symbol attributes go here for code generation */
	struct 	sym_entry *next;
} *SymbolTableEntry;

/*
 * Prototypes
 */
SymbolTable new_st(char *name, int size); /* create symbol table */
void delete_st(SymbolTable); /* destroy symbol table */
int insert_sym(SymbolTable, char *, Typeptr); /* enter symbol into table */
SymbolTableEntry lookup_st(SymbolTable, char *); /* lookup symbol */
int enter_newscope(char *s, Typeptr tp);

void create_builtin_packages(SymbolTable global);
void check_qualified_name(SymbolTable global, struct tree *n);
void type_checker(struct tree *n);
void check_method_arg(struct tree *func_node, Paramlist method_call_params);
void add_stringpool(struct tree *n);
void print_stringpool();

void class_declaration(struct tree *n);
void func_declaration(struct tree *n);

extern SymbolTable globals; /* global symbols */
extern SymbolTable current; /* current */
extern struct string_list *stringpool;

extern void printsymbols(SymbolTable st, int level);

void populate_symboltables(struct tree * n);
void printsyms(struct tree * n);

#define pushscope(stp) do { stp->parent = current; current = stp; } while (0)
#define popscope() do { current = current->parent; } while(0)

#endif
