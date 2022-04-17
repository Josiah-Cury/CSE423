#include <stdio.h>
#include "tac.h"

int main(int argc, char const *argv[]) {
	struct instr *head_inst, *new_inst;
	//printf("Beginning Lab11\n");

	struct addr *call_tmp;

	struct addr loc_i = {R_LOCAL, 0};
	struct addr const_5 = {R_CONST, 5};
	struct addr null_addr = {R_NONE, 0};
	struct addr loc_t1 = {R_LOCAL, 8};
	struct addr const_1 = {R_CONST, 1};
	struct addr loc_t2 = {R_LOCAL, 16};
	struct addr loc_t3 = {R_LOCAL, 24};
	struct addr loc_t4 = {R_LOCAL, 32};
	struct addr loc_t5 = {R_LOCAL, 40};
	struct addr str_0 = {R_STRING, 0};
	struct addr tostr_1 = call_addr("tostr,1");
	struct addr cat_2 = call_addr("cat,2");
	struct addr println_1 = call_addr("println,1");

	// Start head of linked list of instructions
	head_inst = gen(O_ASN, loc_i, const_5, null_addr);

	// Append new instruction
	new_inst = gen(O_MUL, loc_t1, loc_i, loc_i);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_ADD, loc_t2, loc_t1, const_1);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_ASN, loc_i, loc_t2, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_PARM, loc_i, null_addr, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_CALL, tostr_1, loc_t3, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_PARM, str_0, null_addr, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_PARM, loc_t3, null_addr, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_CALL, cat_2, loc_t4, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_PARM, loc_t4, null_addr, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_CALL, println_1, loc_t5, null_addr);
	head_inst = append(head_inst, new_inst);

	new_inst = gen(O_RET, null_addr, null_addr, null_addr);
	head_inst = append(head_inst, new_inst);

	tacprint(head_inst);

	return 0;
}
