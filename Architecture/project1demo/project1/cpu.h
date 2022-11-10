/*********************************************************************************
* 
*  Created by Xiang on 2020/3/9.
*  Copyright (c) 2020-2021 Xiang. All rights reserved.
* 
**********************************************************************************/
#ifndef _CPU_H_
#define _CPU_H_
/**
 *  cpu.h
 *  Contains various CPU and Pipeline Data structures
 */

#include <stdbool.h>
#include "instruction.h"

#define MEMORY_SIZE 16*1024
/* Set this flag to 1 to enable debug(log) messages */
/* When running the progrm with DEBUGLOG=1, use libpipeline_debug.a instead of libpipeline.a */
#define DEBUGLOG 1
/* Register read port */
#define READ_PORT 1

enum
{
	F,           // Pipeline stage: Fetch
	DRF,         // Pipeline stage: Decode
	EXa,         // Pipeline stage: Exa
    EXb,         // Pipeline stage: Exb1
	EXb2,        // Pipeline stage: Exb2
	MEM,         // Pipeline stage: Mem1
	MEM2,        // Pipeline stage: Mem2
	MEM3,        // Pipeline stage: Mem3
	MEM4,        // Pipeline stage: Mem4
	WB,          // Pipeline stage: WB
	NUM_STAGES   
};


typedef enum {
    SIMULATE,  // Debug: simulation mode
    DISPLAY,   // Debug: print internal CPU status
} CommandType;

typedef enum {
    NotWaiting, // There is no outstanding branch instruction in the pipeline
    Waiting,    // fetch stage waits for the completion of branch instruction 
    Taken,      // branch is taken
    NotTaken    // branch is not taken
} BranchType;


/* Model of CPU stage latch */
typedef struct CPU_Stage
{
	int pc;             // Program Counter
	char opcode[128];	// Operation Code
	int buffer;         // Internal buffer for EX and MEM stage. 
	                    // No need to touch in this project. 
	int imm;		    // Literal Value: holds literal value read from codes
	                    // The literal value must be copied to either operand (rs1 or rs2) in the decode stage. 
    bool is_imm;        // whether is imm
    char trace[128];    // Log

	int rd;		        // Destination Register Address (e.g., R0, R1, R2, ...)
	int rs1;		    // Source-1 Register Address (e.g., R0, R1, R2, ...)
	int rs2;		    // Source-2 Register Address (e.g., R0, R1, R2, ...)
    int rd_value;   	// Destination Register Value
	int rs1_value;		// Source-1 Register Value
	int rs2_value;		// Source-2 Register Value
    bool read_rs1_done; // Destination Register read is successfully completed
    bool read_rs2_done; // Source-1 Register read is successfully completed
	bool read_rd_done;  // Source-2 Register read is successfully completed

	int valid;          // Flag to indicate the stage has valid instruction
	int stalled;        // Flag to indicate the stage is stalled

} CPU_Stage;

//
// The CPU will have numbers of registers assigned to this project. (*regs)
// When a register is updated at WB stage, is_writing value is set to TRUE. 
// 
typedef struct Register
{
    int value;          // contains register value
    bool is_writing;    // indicate that the register is current being written
	                    // True: register is not ready
						// False: register is ready
} Register;

/* Model of CPU */
typedef struct CPU
{
	/* Clock cycles elasped */
	int clock;
	/* Deadlock watcher: it quits simulation after this clock. 
	   Currently clock_stop is set to 80000 */
    int clock_stop;
	/* The number of executed instructions during simulation */
	int completed_instructions;

	/* Current program counter */
	int pc;
    
	/* For debugging: prints CPU internal status */
    CommandType commandType;

	/* Integer register file */
	Register *regs;

	/* Array of 5 CPU_stage */
	CPU_Stage stage[NUM_STAGES];

	/* Code Memory where instructions are stored */
	CPU_Instruction* code_memory;
	int code_memory_size;

	/* Data Memory */
	int data_memory[MEMORY_SIZE];

	/* No need to touch: internal use */
	int ins_completed;
    
	/* Indicate whether a current branch is taken or non-taken */
    BranchType branch;
    
	/* Count register reads during execution */
    int read_count;
    
	/* No need to touch: log file */
    FILE *f;
    char *log_path;
    
	/* No need to touch: internal use */
    int up_date_reg;
    
	// data hazard count during execution
    int data_hazard_count;
	// control hazard count during execution
    int control_hazard_count;
	// structural hazard count during execution
    int structural_hazard_count;
} CPU;

CPU*
CPU_init(const char* filename);

Register* create_registers(int size);
CPU_Instruction*create_code_memory(const char* filename, int* size);

int
CPU_run(CPU* cpu);

void
CPU_stop(CPU* cpu);

void
init_globals(CPU* cpu); //initial global function

void
init(void); 

int
fetch(CPU* cpu);

int
decode(CPU* cpu);

int
memory(CPU* cpu, int times);

int
writeback(CPU* cpu);

CPU_Instruction* 
get_cur_instruction(CPU* cpu);

void
print_stage_content(char* name, CPU_Stage* stage, FILE *f);

bool 
is_branch(char *opcode); //checks if givrn instruction is branch or not

void 
count_one(int *count); // increment counter by 1

#endif
