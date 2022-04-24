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
	int i, j;
	struct instr *g;
	struct addr null_addr = {R_NONE, 0};
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
		case PR_ADD_EXPR_SUB: {
			t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
			t->address = new_temp(t->symtab->byte_count);
			//printf("BYTE COUNT FOR CURRENT SCOPE: %d\n", t->symtab->byte_count);
			t->symtab->byte_count += 8;
			//printf("BYTE COUNT FOR CURRENT SCOPE: %d\n", t->symtab->byte_count);

			g = gen(O_ADD, *t->address, *t->kids[0]->address,
				*t->kids[1]->address);

			t->icode = concat(t->icode, g);
			break;
		}

		case PR_ASSIGNMENT: {
			t->address = t->kids[0]->address;
			g = gen(O_ASN, *t->kids[0]->address, *t->kids[2]->address, null_addr);
			t->icode = concat(t->kids[2]->icode, g);
			printf("leftside: %s | rightside: %s\n", print_addr(*t->kids[0]->address), print_addr(*t->kids[2]->address));

			printf("Successfully did assignment! %s:%d\n", t->symbolname, t->prodrule);
			print_instr(t->icode);
			break;
		}

		case TOKEN: {
			// if (t->leaf->category == IDENTIFIER) {
			// 	printf("Found %s:%s\n", t->leaf->text, print_addr(*t->address));
			// }
			if (t->leaf->category == INTEGER_LITERAL) {
				t->address = gen_int_addr(t->leaf->ival);
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

struct addr call_addr(char *s) {
	struct addr a;
	a.region = R_PROCCALL;
	a.u.name = s;
	return a;
}

struct addr *gen_int_addr(int ival) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_CONST;
	a->u.offset = ival;
	return a;
}

struct addr *gen_local_addr(SymbolTable st) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_LOCAL;
	a->u.offset = st->byte_count;
	return a;
}

struct instr *gen(int op, struct addr a1, struct addr a2, struct addr a3)
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

char *print_addr(struct addr a) {
	char *r = regionname(a.region);
	char buf[100];
	char *s;
	if (a.region == R_NONE)
		return 0;
	if (a.region == R_PROCCALL){
		sprintf(buf,"%s", a.u.name);
		printf("%s\n", a.u.name);
		s = malloc(sizeof(buf));
		return s;
	}
	sprintf(buf, "%s:%d", r, a.u.offset);
	//printf("Test 2: %s\n", buf);
	s = malloc(sizeof(buf)+1);
	s = buf;
	//printf("test 1: %s\n", s);
	return s;
}

void print_instr(struct instr *rv) {
	char *o;

	o = opcodename(rv->opcode);
	printf("%s\t", o);
	printf("%s", print_addr(rv->dest));

	if (rv->src1.region != R_NONE) {
		printf(",");
		printf("%s", print_addr(rv->src1));
	}
	if (rv->src2.region != R_NONE) {
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
