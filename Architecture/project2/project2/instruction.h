#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <stdbool.h>


typedef struct CPU_Instruction
{
	char opcode[128];	// Operation Code
	int rd;		        // Destination Register Address
	int rs1;		    // Source-1 Register Address
	int rs2;		    // Source-2 Register Address
	int imm;		    // Literal Value
    bool is_imm;        // whether is imm
    char trace[128];    // Logging
} CPU_Instruction;

void create_CPU_instruction(CPU_Instruction* ins, char* buffer);

void print_Instruction(CPU_Instruction* ins);

#endif
