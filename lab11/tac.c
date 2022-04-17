/*
 * Three Address Code - skeleton for CS 423
 */
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

int labelcounter;

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

/*struct instr *genMethodCall(int op, struct addr a1, struct addr a2, struct addr a3)
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
}*/

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
	if (a.region == R_NONE)
		return 0;
	if (a.region == R_PROCCALL){
		printf("%s", a.u.name);
		return 0;
	}
	printf("%s:%d", r, a.u.offset);
}

void print_instr(struct instr *rv) {
	char *o;

	o = opcodename(rv->opcode);
	printf("%s\t", o);
	print_addr(rv->dest);

	if (rv->src1.region != R_NONE)
		printf(",");
	print_addr(rv->src1);
	if (rv->src2.region != R_NONE)
		printf(",");
	print_addr(rv->src2);

	printf("\n");
}

void tacprint(struct instr *rv) {
	struct instr *ltmp = rv;
	while(ltmp != NULL) {
		print_instr(ltmp);
		ltmp = ltmp->next;
	}
}
