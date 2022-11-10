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
 int count =0;
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
    	cpu->control_hazard_count++;
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
    
    CPU_Stage drfstage = cpu->stage[DRF];
    stage->stalled = drfstage.stalled ? 1 : 0;
    
    if(!drfstage.stalled){
        CPU_Stage* next_stage = &cpu->stage[DRF];
        cpu->stage[DRF] = cpu->stage[F];
    }
    
    stage->valid = 0;
    
    if(drfstage.stalled){
        stage->valid = 1;
    }

    print_stage_content("Fetch", stage, cpu->f);
    return 0;
}

/*
 *  Decode Stage of CPU Pipeline
 */
int
decode(CPU* cpu)
{
    CPU_Stage* fstage = &cpu->stage[F];
    CPU_Stage* stage = &cpu->stage[DRF];
    CPU_Stage drfstage = cpu->stage[DRF];
    
    if (stage->valid) {
   	if(drfstage.rs1 != -1 && drfstage.rs2 != -1){
   		if(!cpu->regs[stage->rs1].is_writing && !cpu->regs[stage->rs2].is_writing){
   			if(!stage->read_rs1_done && !stage->read_rs2_done){
   			    stage->rs1_value = cpu->regs[stage->rs1].value;
   			    stage->read_rs1_done = true;
   			    cpu->structural_hazard_count++;
   			    stage->stalled = 1;
   			    stage->valid = 1;
   			    cpu->regs[stage->rd].is_writing = false;
   			    print_stage_content("Decode", stage, cpu->f);
   			    return 0;
   			}
   			
   			stage->rs1_value = cpu->regs[stage->rs1].value;
   			stage->rs2_value = cpu->regs[stage->rs2].value;
   			stage->read_rs1_done = true;
   			stage->read_rs2_done = true;
   			stage->stalled = 0;
   			cpu->regs[stage->rd].is_writing = true;
   		}else{
   			if(!cpu->regs[stage->rs1].is_writing && cpu->regs[stage->rs2].is_writing){
   			    stage->rs1_value = cpu->regs[stage->rs1].value;
   			    stage->read_rs1_done = true;
   			}
   			if(cpu->regs[stage->rs1].is_writing && !cpu->regs[stage->rs2].is_writing){
   			    stage->rs2_value = cpu->regs[stage->rs2].value;
   			    stage->read_rs2_done = true;
   			}
   			stage->stalled = 1;
   			stage->valid = 1;
   			cpu->data_hazard_count++;
   			cpu->regs[stage->rd].is_writing = false;
   			print_stage_content("Decode", stage, cpu->f);
   			return 0;
   		}
   	}else if(drfstage.rs1 != -1 && drfstage.rs2 == -1){
   		if(!cpu->regs[stage->rs1].is_writing){
   			stage->rs1_value = cpu->regs[stage->rs1].value;
   			stage->read_rs1_done = true;
   			stage->rs2_value = stage->imm;
   			stage->stalled = 0;
   			cpu->regs[stage->rd].is_writing = true;
   		}else{
   			stage->stalled = 1;
   			stage->valid = 1;
   			cpu->data_hazard_count++;
   			cpu->regs[stage->rd].is_writing = false;
   			print_stage_content("Decode", stage, cpu->f);
   			return 0;
   		}
   	}else if(drfstage.rs1 == -1 && drfstage.rs2 == -1){
   		if(!stage->is_imm){
   		    stage->read_rs1_done = true;
   		}
   		if(is_branch(stage->opcode)){
   		    cpu->branch = Waiting;
   		    if(!cpu->regs[stage->rd].is_writing){
   		    	stage->rd_value = cpu->regs[stage->rd].value;
   		    	stage->read_rd_done = true;
   		    }else{
   		    	cpu->data_hazard_count++;
   		    	stage->stalled = 1;
   		    	stage->valid = 1;
   		    	print_stage_content("Decode", stage, cpu->f);
   			return 0;
   		    }
   		}
   		stage->rs1_value = stage->imm;
   		stage->stalled = 0;
   		if(!is_branch(stage->opcode)){
   		    cpu->regs[stage->rd].is_writing = true;
   		}
   	}
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
		
		cpu->stage[EXa] = cpu->stage[DRF];
		stage->valid = 0;
		print_stage_content("Decode", stage, cpu->f);
	}
    return 0;
}
