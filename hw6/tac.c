#include <stdio.h>
#include <stdlib.h>
#include "tac.h"

char *regionnames[] = {"global","loc", "class", "lab", "const", "", "none", "proccall", "str"};
char *regionname(int i) { return regionnames[i-R_GLOBAL]; }
char *opcodenames[] = {
	"ADD","SUB", "MUL", "DIV", "NEG", "ASN", "ADDR", "LCONT", "SCONT", "GOTO",
	"BLT", "BLE", "BGT", "BGE", "BEQ", "BNE", "BIF", "BNIF", "PARM", "CALL",
	"RETURN"
};
char *opcodename(int i) { return opcodenames[i-O_ADD]; }
char *pseudonames[] = {
	"glob","proc", "loc", "lab", "end", "prot"
};
char *pseudoname(int i) { return pseudonames[i-D_GLOB]; }

int labelcounter = 0;

void codegen(struct tree * t) {
	int i;
	struct instr *g;
	struct addr *null_addr = gen_int_addr(R_NONE, 0);
	struct addr *a1, *a2, *a3;
	struct tree *node;
	SymbolTableEntry ste;

	if (t==NULL) return;

	/*
	* this is a post-order traversal, so visit children first
	*/
	for(i=0;i<t->nkids;i++)
		codegen(t->kids[i]);

	/*
	* back from children, consider what we have to do with
	* this node. The main thing we have to do, one way or
	* another, is assign t->code
	*/
	switch (t->prodrule) {
		case PR_ADD_EXPR_ADD:
		case PR_ADD_EXPR_SUB:
		case PR_MUL_EXPR_MULT:
		case PR_MUL_EXPR_DIV: {
			t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
			t->address = new_temp(t->symtab->byte_count);
			//printf("BYTE COUNT FOR CURRENT SCOPE: %d\n", t->symtab->byte_count);
			t->symtab->byte_count += 8;
			//printf("BYTE COUNT FOR CURRENT SCOPE: %d\n", t->symtab->byte_count);

			if (t->prodrule == PR_ADD_EXPR_ADD) {
				g = gen(O_ADD, t->address, t->kids[0]->address,
					t->kids[1]->address);
			} else if (t->prodrule == PR_ADD_EXPR_SUB) {
				g = gen(O_SUB, t->address, t->kids[0]->address,
					t->kids[1]->address);
			} else if (t->prodrule == PR_MUL_EXPR_MULT) {
				g = gen(O_MUL, t->address, t->kids[0]->address,
					t->kids[1]->address);
			} else if (t->prodrule == PR_MUL_EXPR_DIV) {
				g = gen(O_DIV, t->address, t->kids[0]->address,
					t->kids[1]->address);
			}


			t->icode = concat(t->icode, g);
			break;
		}

		case PR_ASSIGNMENT_DECL: {
			t->address = t->kids[1]->address;
			g = gen(O_ASN, t->kids[1]->address, t->kids[3]->address, null_addr);
			t->icode = concat(t->kids[3]->icode, g);

			break;
		}

		case PR_ASSIGNMENT: {
			t->address = t->kids[0]->address;
			g = gen(O_ASN, t->kids[0]->address, t->kids[2]->address, null_addr);
			t->icode = concat(t->kids[2]->icode, g);

			break;
		}

		case PR_METHOD_DECLARATOR: {
			a1 = malloc(sizeof(struct addr));
			a2 = malloc(sizeof(struct addr));
			a3 = malloc(sizeof(struct addr));
			a1->u.name = t->kids[0]->leaf->text;
			a2->u.offset = 0;
			a3->u.offset = t->symtab->byte_count;
			g = gen(D_PROC, a1, a2, a3);
			t->icode = concat(g, t->kids[0]->icode);
			break;
		}

		case PR_BLOCK: {
			g = gen(O_RET, null_addr, null_addr, null_addr);
			t->icode = concat(t->kids[0]->icode, g);
			break;
		}

		case PR_METHODCALL_P: {
			if (t->kids[0]->prodrule != PR_QUALIFIED_NAME) {
				ste = lookup_st(t->kids[1]->symtab, t->kids[1]->leaf->text);
				t->kids[1]->address = ste->address;
				//printf("%s\n", print_addr(t->kids[1]->address));
				g = gen(O_PARM, t->kids[1]->address, null_addr, null_addr);
				t->icode = g;
			} else {
				node = t->kids[0];
				while (node->kids[1]->prodrule == PR_QUALIFIED_NAME) {
					node = node->kids[1];
				}
				//printf("QualifiedName Function: %s Parameter: %s\n", node->kids[1]->leaf->text, t->kids[1]->leaf->text);
			}

			break;
		}

		case TOKEN: {
			// if (t->leaf->category == IDENTIFIER) {
			// 	printf("Found %s:%s\n", t->leaf->text, print_addr(*t->address));
			// }
			if (t->leaf->category == INTEGER_LITERAL) {
				t->address = gen_int_addr(R_CONST, t->leaf->ival);
				//printf("Found %d:%s\n", t->leaf->ival, print_addr(*t->address));
			}
		}

		/*
		* ... really, a bazillion cases, up to one for each
		* production rule (in the worst case)
		*/
		default:
			/* default is: concatenate our children's code */
			t->icode = NULL;
			for(i=0;i<t->nkids;i++) {
				if (t->kids[i] != NULL) {
					t->icode = concat(t->icode, t->kids[i]->icode);
				}
			}
	}
}

struct addr *genlabel()
{
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_LABEL;
	a->u.offset = labelcounter++;
	printf("generated a label %d\n", a->u.offset);
	return a;
}

struct addr *call_addr(char *s) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_PROCCALL;
	a->u.name = s;
	return a;
}

struct addr *gen_int_addr(int region, int ival) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = region;
	a->u.offset = ival;
	return a;
}

struct addr *gen_local_addr(SymbolTable st) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_LOCAL;
	a->u.offset = st->byte_count;
	return a;
}

struct instr *gen(int op, struct addr *a1, struct addr *a2, struct addr *a3)
{
	struct instr *rv = malloc(sizeof (struct instr));
	if (rv == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(4);
	 }
	rv->opcode = op;
	rv->dest = a1;
	rv->src1 = a2;
	rv->src2 = a3;
	rv->next = NULL;
	return rv;
}

struct addr *new_temp(int n) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_LOCAL;
	a->u.offset = n;
	return a;
}

struct instr *copylist(struct instr *l)
{
	if (l == NULL) return NULL;
	struct instr *lcopy = gen(l->opcode, l->dest, l->src1, l->src2);
	lcopy->next = copylist(l->next);
	return lcopy;
}

struct instr *append(struct instr *l1, struct instr *l2)
{
	if (l1 == NULL) return l2;
	struct instr *ltmp = l1;
	while(ltmp->next != NULL) ltmp = ltmp->next;
	ltmp->next = l2;
	return l1;
}

struct instr *concat(struct instr *l1, struct instr *l2)
{
	return append(copylist(l1), l2);
}

char *print_addr(struct addr *a) {
	char *r = regionname(a->region);
	char buf[100];
	char *s;

	if (a->region == R_NONE)
		return 0;
	if (a->region == R_PROCCALL){
		sprintf(buf,"%s", a->u.name);
		printf("%s\n", a->u.name);
		s = malloc(sizeof(buf));
		return s;
	}
	sprintf(buf, "%s:%d", r, a->u.offset);
	//printf("Test 2: %s\n", buf);
	s = malloc(sizeof(buf)+1);
	s = buf;
	//printf("test 1: %s\n", s);
	return s;
}

void print_instr(struct instr *rv) {
	char *o;
	//printf("WE BE PRINTING INSTR HERE\n");
	if(rv == NULL) { return; }

	if (rv->opcode == D_PROC) {
		o = pseudoname(rv->opcode);
		printf("%s ", o);
		printf("%s,%d,%d\n", rv->dest->u.name, rv->src1->u.offset, rv->src2->u.offset);
		return;
	}
	printf("\t");
	o = opcodename(rv->opcode);
	printf("%s\t", o);

	if (rv->dest->region != R_NONE) { printf("%s", print_addr(rv->dest)); }

	if (rv->src1->region != R_NONE) {
		printf(",");
		printf("%s", print_addr(rv->src1));
	}

	if (rv->src2->region != R_NONE) {
		printf(",");
		printf("%s", print_addr(rv->src2));
	}

	printf("\n");
}

void tacprint(struct instr *rv) {
	struct instr *ltmp = rv;
	while(ltmp != NULL) {
		print_instr(ltmp);
		ltmp = ltmp->next;
	}
}
