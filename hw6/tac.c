#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"

char *regionnames[] = {"global","loc", "class", "lab", "const", "", "none", "proccall", "str"};
char *regionname(int i) { return regionnames[i-R_GLOBAL]; }
char *opcodenames[] = {
	"ADD","SUB", "MUL", "DIV", "NEG", "ASN", "ADDR", "LCONT", "SCONT", "GOTO",
	"BLT", "BLE", "BGT", "BGE", "BEQ", "BNE", "BIF", "BNIF", "PARM", "CALL",
	"RETURN", "LT", "LE", "GT", "GE", "EQ", "NEQ", "AND", "OR", "NOT"
};
char *opcodename(int i) { return opcodenames[i-O_ADD]; }
char *pseudonames[] = {
	"glob","proc", "loc", "lab", "end", "prot"
};
char *pseudoname(int i) { return pseudonames[i-D_GLOB]; }

int labelcounter = 0;

void codegen(struct tree * t) {
	int i, j;
	struct instr *g = NULL;
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
		case PR_UNARY_EXPR_NEG:
			t->address = new_temp(t->symtab);
			g = gen(O_NEG, t->address, t->kids[1]->address, null_addr);
			t->icode = concat(t->kids[1]->icode, g);
			break;

		case PR_UNARY_EXPR_NOT:
			t->address = new_temp(t->symtab);
			g = gen(O_NOT, t->address, t->kids[1]->address, null_addr);
			t->icode = concat(t->kids[1]->icode, g);
			break;

		case PR_ADD_EXPR_ADD:
		case PR_ADD_EXPR_SUB:
		case PR_MUL_EXPR_MULT:
		case PR_MUL_EXPR_DIV: {
			t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);

			if (t->prodrule == PR_ADD_EXPR_ADD) {
				if (t->type->basetype == STRING_TYPE) {

					g = int_tostr(t->kids[0], t->kids[0]->symtab);
					g = concat(g, gen(O_PARM, t->kids[0]->address, null_addr, null_addr));
					g = concat(g, int_tostr(t->kids[1], t->kids[1]->symtab));
					g = concat(g, gen(O_PARM, t->kids[1]->address, null_addr, null_addr));


					// if (t->kids[0]->type->basetype == STRING_TYPE) {
					// 	g = str_to_addr(t->kids[0], t->kids[0]->symtab);
					// } else {
					// 	g = int_tostr(t->kids[0], t->kids[0]->symtab);
					// }
					// g = concat(g, gen(O_PARM, t->kids[0]->address, null_addr, null_addr));
					//
					// if (t->kids[1]->type->basetype == STRING_TYPE) {
					// 	g = concat(g, str_to_addr(t->kids[1], t->kids[1]->symtab));
					// } else {
					// 	g = concat(g, int_tostr(t->kids[1], t->kids[1]->symtab));
					// }
					// g = concat(g, gen(O_PARM, t->kids[1]->address, null_addr, null_addr));


					a1 = gen_string_addr(R_PROCCALL, "cat");
					a2 = gen_int_addr(R_PARAMNUM, 2);
					a3 = new_temp(t->symtab);

					t->address = a3;
					g = concat(g, gen(O_CALL, a1, a2, a3));
					// printf("Maybe this is the problem!\n");
					// tacprint(g);
				} else {
					t->address = new_temp(t->symtab);
					g = gen(O_ADD, t->address, t->kids[0]->address,
						t->kids[1]->address);
					// printf("QUICK CHECK\n");
					// print_instr(g);
				}
			} else if (t->prodrule == PR_ADD_EXPR_SUB) {
				t->address = new_temp(t->symtab);
				g = gen(O_SUB, t->address, t->kids[0]->address,
					t->kids[1]->address);
			} else if (t->prodrule == PR_MUL_EXPR_MULT) {
				t->address = new_temp(t->symtab);
				// printf("LeftHandSide: %s\n", print_addr(t->kids[0]->address));
				// printf("RightHandSide: %s\n", print_addr(t->kids[1]->address));
				g = gen(O_MUL, t->address, t->kids[0]->address,
					t->kids[1]->address);
			} else if (t->prodrule == PR_MUL_EXPR_DIV) {
				t->address = new_temp(t->symtab);
				g = gen(O_DIV, t->address, t->kids[0]->address,
					t->kids[1]->address);
			}


			t->icode = concat(t->icode, g);
			break;
		}

		case PR_REL_EXPR: {
			t->icode = concat(t->kids[0]->icode, t->kids[2]->icode);
			t->address = new_temp(t->symtab);
			switch (t->kids[1]->leaf->category) {
				case '<':
					g = gen(O_LT, t->address, t->kids[0]->address, t->kids[2]->address);
					break;
				case '>':
					g = gen(O_GT, t->address, t->kids[0]->address, t->kids[2]->address);
					break;
				case GREATER_EQUAL:
					g = gen(O_GE, t->address, t->kids[0]->address, t->kids[2]->address);
					break;
				case LESS_EQUAL:
					g = gen(O_LE, t->address, t->kids[0]->address, t->kids[2]->address);
					break;
				default:
					g = NULL;
					break;
			}

			t->icode = concat(t->icode, g);
			break;
		}

		case PR_COND_AND_EXPR:
		case PR_COND_OR_EXPR:
			t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
			t->address = new_temp(t->symtab);

			switch (t->prodrule) {
				case PR_COND_AND_EXPR:
					g = gen(O_AND, t->address, t->kids[0]->address, t->kids[1]->address);
					break;
				case PR_COND_OR_EXPR:
					g = gen(O_OR, t->address, t->kids[0]->address, t->kids[1]->address);
					break;
			}

			t->icode = concat(t->icode, g);
			break;

		case PR_ASSIGNMENT_DECL: {
			t->address = t->kids[1]->address;
			g = gen(O_ASN, t->kids[1]->address, t->kids[3]->address, null_addr);
			t->icode = concat(t->kids[3]->icode, g);

			break;
		}

		case PR_ASSIGNMENT_UNARY: {
			t->address = t->kids[0]->address;
			a1 = new_temp(t->symtab);
			a2 = gen_int_addr(R_CONST, 1);
			switch (t->kids[1]->leaf->category) {
				case INCREMENT:
					g = gen(O_ADD, a1, t->address, a2);
					g = concat(g, gen(O_ASN, t->address, a1, null_addr));
					break;
				case DECREMENT:
					g = gen(O_SUB, a1, t->address, a2);
					g = concat(g, gen(O_ASN, t->address, a1, null_addr));
					break;
			}

			t->icode = concat(t->kids[0]->icode, g);
			break;
		}

		case PR_ASSIGNMENT: {
			t->address = t->kids[0]->address;
			g = gen(O_ASN, t->kids[0]->address, t->kids[2]->address, null_addr);
			t->icode = concat(t->kids[2]->icode, g);

			break;
		}

		case PR_METHOD_DECLARATOR: {
			ste = lookup_st(t->symtab, t->kids[0]->leaf->text);

			//printf("(SymbolTableName : %s,%d)\n", ste->type->u.f.st->name, ste->type->u.f.nparams);

			a1 = gen_string_addr(R_NAME, t->kids[0]->leaf->text);
			a2 = gen_int_addr(R_PARAMNUM, ste->type->u.f.nparams);
			a3 = gen_int_addr(R_PARAMNUM, t->symtab->byte_count);

			g = gen(D_PROC, a1, a2, a3);
			t->icode = concat(g, t->kids[0]->icode);

			break;
		}

		//ADD DIFFERENT RETURN FOR RETURNSTMT TREE NODE!!!!!!!!

		case PR_METHOD_DECL: {
			if (strcmp(t->kids[0]->kids[2]->leaf->text, "void")) {
				g = gen(O_RET, null_addr, null_addr, null_addr);
				t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
				t->icode = concat(t->icode, g);
			} else {
				g = gen(O_RET, null_addr, null_addr, null_addr);
				t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
				t->icode = concat(t->icode, g);
			}

			break;
		}

		case PR_METHODCALL_P: {
			if (t->kids[0]->prodrule != PR_QUALIFIED_NAME) {
				ste = lookup_st(t->symtab, t->kids[0]->leaf->text);

				a1 = gen_string_addr(R_PROCCALL, t->kids[0]->leaf->text);
				a2 = gen_int_addr(R_PARAMNUM, ste->type->u.f.nparams);
				a3 = new_temp(t->kids[1]->symtab);
				//printf("%s\n", print_addr(t->kids[1]->address));

				if (t->kids[1]->prodrule != PR_ARG_LIST) {
					g = gen(O_PARM, t->kids[1]->address, null_addr, null_addr);
					t->icode = concat(t->kids[1]->icode, g);
				} else {
					node = t->kids[1];
					while (node->kids[0]->prodrule == PR_ARG_LIST) {
						g = gen(O_PARM, node->kids[1]->address, null_addr, null_addr);
						t->icode = concat(t->icode, g);
						node = node->kids[0];
					}
					g = gen(O_PARM, node->kids[0]->address, null_addr, null_addr);
					t->icode = concat(t->icode, g);

					g = gen(O_PARM, node->kids[1]->address, null_addr, null_addr);
					t->icode = concat(t->icode, g);
				}


				g = gen(O_CALL, a1, a2, a3);
				t->address = a3;
				t->icode = concat(t->kids[1]->icode, t->icode);
				t->icode = concat(t->icode, g);
			} else {
				node = t->kids[0];
				while (node->kids[1]->prodrule == PR_QUALIFIED_NAME) {
					node = node->kids[1];
				}
				// ste = lookup_st(t->symtab, node->kids[1]->leaf->text);
				j = 0;
				//printf("%s\n", print_addr(t->kids[1]->address));
				if (t->kids[1]) {
					if ((g = str_to_addr(t->kids[1], t->kids[1]->symtab))) {
						//printf("QualifiedName: %s\n", print_addr(t->kids[1]->address));
						t->icode = concat(t->icode, g);
					}

					g = gen(O_PARM, t->kids[1]->address, null_addr, null_addr);
					t->icode = concat(t->icode, t->kids[1]->icode);
					j = 1;
				}

				t->icode = concat(t->icode, g);

				a1 = gen_string_addr(R_PROCCALL, node->kids[1]->leaf->text);
				a2 = gen_int_addr(R_PARAMNUM, j);
				a3 = new_temp(t->symtab);

				g = gen(O_CALL, a1, a2, a3);
				t->icode = concat(t->icode, g);
				t->address = a3;
			}

			break;
		}

		case PR_ARG_LIST: {
			t->icode = concat(t->kids[0]->icode, t->kids[1]->icode);
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

			if (t->leaf->category == IDENTIFIER) {
				ste = lookup_st(t->symtab, t->leaf->text);
				if (ste)
					t->address = ste->address;
			}
			break;
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

struct addr *genlabel() {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = R_LABEL;
	a->u.offset = labelcounter++;
	printf("generated a label %d\n", a->u.offset);
	return a;
}

struct addr *gen_string_addr(int region, char *s) {
	struct addr *a = malloc(sizeof(struct addr));
	a->region = region;
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
	return gen_int_addr(R_LOCAL, st->byte_count);
}

struct addr *new_temp(SymbolTable st) {
	struct addr *a = gen_local_addr(st);
	//printf("%s\n", print_addr(a));
	st->byte_count += 8;
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

struct instr *str_to_addr(struct tree *node, SymbolTable st) {
	struct addr *tmp, *old_addr = node->address, *null_addr = gen_int_addr(R_NONE, 0);

	if (old_addr->region == R_STRING) {
		tmp = new_temp(st);
		node->address = tmp;
		return gen(O_ADDR, tmp, old_addr, null_addr);
	}

	return NULL;
}

struct instr *int_tostr(struct tree *node, SymbolTable st) {
	struct addr *tmp, *old_addr = node->address,
	 	*null_addr = gen_int_addr(R_NONE, 0);

	if (old_addr->region == R_STRING) {
		tmp = new_temp(st);
		node->address = tmp;
		return gen(O_ADDR, tmp, old_addr, null_addr);
	} else if (old_addr->region == R_LOCAL && node->type->basetype != STRING_TYPE) {
		struct addr *a1, *a2;
		struct instr *g;

		a1 = gen_string_addr(R_PROCCALL, "tostr");
		a2 = gen_int_addr(R_PARAMNUM, 1);
		tmp = new_temp(st);
		node->address = tmp;
		g = concat(gen(O_PARM, old_addr, null_addr, null_addr),
			gen(O_CALL, a1, a2, tmp));
		return g;
	}

	return NULL;
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

	if (a->region == R_PROCCALL || a->region == R_NAME){
		sprintf(buf,"%s", a->u.name);
		//printf("%s\n", a->u.name);
		s = malloc(strlen(buf));
		memcpy(s, buf, strlen(buf));
		return s;
	}

	if (a->region == R_PARAMNUM) {
		sprintf(buf, "%d", a->u.offset);
		s = malloc(strlen(buf));
		memcpy(s, buf, strlen(buf));
		return s;
	}

	sprintf(buf, "%s:%d", r, a->u.offset);
	//printf("Test 2: %s\n", buf);
	s = malloc(strlen(buf)+1);
	memcpy(s, buf, strlen(buf));
	//printf("test 1: %s\n", s);
	return s;
}

void print_instr(struct instr *rv) {
	char *o;
	//printf("WE BE PRINTING INSTR HERE\n");
	if(rv == NULL) { return; }

	if (rv->opcode == D_PROC) {
		o = pseudoname(rv->opcode);
	// 	printf("%s ", o);
	// 	printf("%s,%d,%d\n", rv->dest->u.name, rv->src1->u.offset, rv->src2->u.offset);
	// 	return;
	} else {
		printf("\t");
		o = opcodename(rv->opcode);
	}

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
	print_stringpool();
	printf("\n");

	printf(".code\n");
	struct instr *ltmp = rv;
	while(ltmp != NULL) {
		print_instr(ltmp);
		ltmp = ltmp->next;
	}
}
