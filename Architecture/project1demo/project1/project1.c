/*********************************************************************************
* 
*  CS 520, Fall 2021, Project #1
* 
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cpu.h"

/*
 * Global function to initialize any additional data structures you like to use.
 * It is run before the execution of cycles.
 */
void
init_globals(CPU* cpu)
{

}

/*
 *  Fetch Stage of CPU Pipeline
 */
int
fetch(CPU* cpu)
{
    if (cpu->branch == Waiting) {
        return 0;
    }

    CPU_Stage* stage = &cpu->stage[F];
    if (!stage->stalled) {
        /* Fetch an instruction indexed by cpu->pc */
        CPU_Instruction* current_ins = get_cur_instruction(cpu);
        if (current_ins == NULL)
            return 0;

        /* Fetch stage retrieve the current instruction's information. */
        stage->pc = cpu->pc;
        strcpy(stage->opcode, current_ins->opcode);
        strcpy(stage->trace, current_ins->trace);
        stage->rd = current_ins->rd;
        stage->rs1 = current_ins->rs1;
        stage->rs2 = current_ins->rs2;
        stage->imm = current_ins->imm;
        stage->is_imm = current_ins->is_imm;
        stage->rd = current_ins->rd;
        stage->read_rs1_done = false;
        stage->read_rs2_done = false;
        stage->valid = 1;

        /* Update PC for next instruction */
        cpu->pc += 4;
    }

    CPU_Stage* next_stage = &cpu->stage[DRF];
    cpu->stage[DRF] = cpu->stage[F];
	stage->valid = 0;

    print_stage_content("Fetch", stage, cpu->f);
    return 0;
}

/*
 *  Decode Stage of CPU Pipeline
 */
int
decode(CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[DRF];
    if (stage->valid) {
    		printf("000000\n");
    		printf(stage->trace);
    		printf("\n");
		// Tip #1
		// You need to check dependencies between instructions and calculate hazards here
		// Instructions have 3 or less operands, and each operand must be placed in a correct location. 
		// 
		// stage->rd_value
		// stage->rs1_value
		// stage->rs2_value
		// stage->imm
		//
		// e.g., add r0 r1 r2
		// rd_value is for r0
		// rs1_value is for r1
		// rs2_value is for r2
		// 
		// Note. some instructions have 2 operands. 
		// 
		// e.g., br r0 0x0100
		// rd_value is for r0
		// imm is for 0x0100
		// 
		// You can obtain an instruction opcode by checking the opcode value which is a string. 
		// 
		// e.g., strcmp(stage->opcode, "st")
		// 
		// Registers are defined inside cpu->regs[num]
		// 
		// typedef struct Register
		// {
		//     int value;
		//     bool is_writing;
		// } Register;
		// 
		// Note that some registers are not ready to be used. 
		// You must set and check whether the register is ready or not by checking "is_writing". 
		// if "is_writing" is true, the register value will be updated and it is not yet ready. 
		// if "is_writing" is false, the register value is ready and can be used. 
		// 
		// In decode stage, you must set the is_writing value correctly. 
		// In later stages, the value will be set again once it is processed. 
		// 

		// Tip #2
		// If a hazard is detected, the pipeline needs to be stalled until the dependency is solved
		// Also, you have to count the stalls due to hazard in 
		// 
		// cpu->data_hazard_count
		// cpu->structural_hazard_count
		// cpu->control_hazard_count
		// 
		// Tip #3. 
		// Please check cpu.h and instruction.h files
		// All the necessary variables are already prepared. 
		// 
		print_stage_content("Decode", stage, cpu->f);
		cpu->stage[EXa] = cpu->stage[DRF];
		stage->valid = 0;
	}
    return 0;
}
