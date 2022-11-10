//
//  cpu.h
//  Project2
//
//  Created by Xiang on 2020/1/28.
//  Copyright © 2020年 Xiang. All rights reserved.
//
#ifndef _CPU_H_
#define _CPU_H_
/**
 *  cpu.h
 *  Contains various CPU and Pipeline Data structures
 */

#include <stdbool.h>
#include "instruction.h"

#define MEMORY_SIZE 16*1024
#define PHYNUM 32

/* Set this flag to 1 to enable debug messages */
#define DEBUGLOG 0
/* Register read port */
#define READ_PORT 2
/* Max dynamic instruction id */
#define MAX_INST_ID 1000
/* ROB size */
#define ROB_SIZE 10
/* Reservation size */
#define RESERVATION_SIZE 6
/* Register size */
#define REG_SIZE 16

enum
{
	F,           // Pipeline stage: Fetch
	DRF,         // Pipeline stage: dispatch
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
} BranchWaitType;

typedef enum {
    ActionEXa,
    ActionEXb,
    ActionMemory
} ActionType;

typedef enum {
    RenameStateEmpty,
    RenameStateMapped,
    RenameStateCommit
} RenameState;

typedef struct Forward_Reg
{
    int value;  
    int valid;
}Forward_Reg;

/* Model of CPU stage latch */
typedef struct Stage_Unit
{
    int inst_id;    // keeps distinct instruction id. It can't be larger than 1000. 
	int pc;		    // Program Counter
	char opcode[128];	// Operation Code

	int rd;		        // Destination Register Address
	int rs1;		    // Source-1 Register Address
	int rs2;		    // Source-2 Register Address
	int imm;		    // Literal Value: holds literal value read from codes
	                    // The literal value must be copied to either operand (rs1 or rs2) in the dispatch stage. 
    bool is_imm;        // whether is imm
    char trace[128]; // Log

    int rd_value;   // Destination Register Value
	int rs1_value;	// Source-1 Register Value
	int rs2_value;	// Source-2 Register Value

	int buffer;		// Latch to hold some value
    int valid;          // Flag to indicate, stage is performing some action
    int renamed;    // Registers used by this stage(instruction) are renamed
      
    int find_rd;
    int find_rs1;       //find needed forwarding
    int find_rs2;       //find needed forwarding

    ActionType action_type;
	int rob_id;

} Stage_Unit;


typedef struct CPU_Stage
{
    Stage_Unit stage_units[2];
    int stalled;		// Flag to indicate, stage is stalled
    int unit_length;
} CPU_Stage;

typedef struct Unit_Buffer
{
    Stage_Unit *unit_list;
    int index;
    int length;
} Unit_Buffer;

typedef enum {
    EXECUTING,
    COMPLETED,
} CompletionType;

typedef enum {
    RegStatus_INVALID,
    RegStatus_VALID
} RegStatus;

typedef struct ROB_entry
{
	int destination;// r_id
	int result;     // value
	int exception;  // no exception
	CompletionType completed; // writing type
	int PC;         // PC
	char opcode[128];	// Operation Code
} ROB_entry_t;

typedef struct Arc_Reg
{   
    int value;
    int tag;
    RegStatus status;
} Arc_Reg_t;

typedef struct Snapshot
{
    int phy_reg[16];
    int inst_id[16];
    int id;
} Snapshot_st;


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
	/* Indicate that PC is updated */
	int pc_updated;
    
	/* For debugging: prints CPU internal status */
    CommandType commandType;

    Snapshot_st snapshot_list[8];
    int snapshot_idx;
    
	/* Array of 5 CPU_stage */
	CPU_Stage stages[NUM_STAGES];

	/* Code Memory where instructions are stored */
	CPU_Instruction* code_memory;
	int code_memory_size;

	/* Data Memory */
	int data_memory[MEMORY_SIZE];

	/* No need to touch: internal use */
	int ins_completed;
    
	/* Indicate whether a current branch is taken or non-taken */
    BranchWaitType branch;

	/* No need to touch: log file */
    FILE *f;
    char *log_path;
    
	/* No need to touch: internal use */
    int write_count_num;
    
	// data hazard count during execution
    int data_hazards;
	// control hazard count during execution
    int control_hazards;
    // fetch stall count during execution
    int fetch_stalls;
	// number of failures to reserve an entry in the reservation station
    int full_reservation_count;
	// number of failures to reserve an entry in the reorder buffer
    int full_reorder_count;

	// Architectural registers
	Arc_Reg_t registers[REG_SIZE];

	// ROB buffer
	int ROB_free;
	int ROB_avail;
	ROB_entry_t ROB[ROB_SIZE];

	// tail of ROB
	int ROB_tail; 
	// head of ROB
	int ROB_head;

	Forward_Reg forwarding_reserve[ROB_SIZE];
	Forward_Reg forwarding[ROB_SIZE];
    Unit_Buffer *reserve_buffer;

    bool completed;
} CPU;

CPU*
CPU_init(const char* filename);

Unit_Buffer* 
create_unit_buffer(int unit_size);

CPU_Instruction* 
create_code_memory(const char* filename, int* size);

int 
append_unit(Stage_Unit unit, Unit_Buffer *buffer);

CPU_Stage*
get_target_stage(int stage_index, CPU *cpu, bool reset_unit_length);

int
CPU_run(CPU* cpu);

void
CPU_stop(CPU* cpu);

void
init_globals(CPU* cpu);

int
fetch(CPU* cpu);

int
dispatch(CPU* cpu);

int
issue(CPU* cpu);

CPU_Instruction* 
get_cur_instruction(CPU* cpu);

void
print_stage_content(int stage, int p_index, Stage_Unit *unit, char *ending, FILE *f);

void
printLog(FILE *stream, char *format, ...);

void 
count_one(int *count);

void 
get_p_trace(char *p_trace, Stage_Unit *unit);

void 
remove_at(int idx, Unit_Buffer *buffer);

#endif
