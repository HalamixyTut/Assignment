//
//  cpu.c
//  Project2
//
//  Created by Xiang on 2020/1/28.
//  Copyright © 2020年 Xiang. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cpu.h"
#include "tool.h"

Stage_Unit *retrieve_fetch(CPU *cpu);

void update_rob(Stage_Unit unit, CPU *cpu);
void empty_buffer(Unit_Buffer *buffer);

int issue(CPU *cpu);

void 
get_phy_reg(int *reg, int *remapped, int iid, CPU *cpu);

int 
allocate_phy(CPU *cpu);

int
memory(CPU* cpu, int times);

int
writeback(CPU* cpu);

void printLog(FILE *stream, char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);
    if (DEBUGLOG) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}


CPU*
CPU_init(const char* filename)
{
    if (!filename) {
        return NULL;
    }
    
    CPU* cpu = malloc(sizeof(struct CPU));
    if (!cpu) {
        return NULL;
    }
    
    struct stat st = {0};
    char out_path[100] = "./logs/";
    if (stat(out_path, &st) == -1) {
        mkdir(out_path, 0700);
    }
    char *path = malloc(100);
    strcpy(path, filename);
    char *ssc;
    int l = 0;
    ssc = strstr(filename, "/");
    while(ssc){
        l = (int)strlen(ssc) + 1;
        path = &path[strlen(path)-l+2];
        ssc = strstr(path, "/");
    };
    
    FILE *f;
    
    char *out_log = replaceWord(path,"asm","log");
    strcat(out_path, out_log);
    cpu->log_path = malloc(100);
    strcpy(cpu->log_path, out_path);
    
    f = fopen(out_path, "w");
    if (f == NULL) {
        printf("Open log file Error!");
        return NULL;
    }
    cpu->f = f;
    
    /* Initialize PC, Registers and all pipeline stages */
    cpu->clock = 0;
    cpu->pc = 0;
	cpu->pc_updated = false;
    cpu->clock_stop = 80000;
    // cpu->snapshot_idx =0;
    cpu->completed = false;
    // value; count; read_count; write_count; is_writing;
    
    memset(cpu->stages, 0, sizeof(CPU_Stage) * NUM_STAGES);
    memset(cpu->data_memory, 0, sizeof(int) * MEMORY_SIZE);
    load_memory_map(cpu->data_memory, "memory_map.txt");
    
    cpu->reserve_buffer = create_unit_buffer(RESERVATION_SIZE);

    /* Parse input file and create code memory */
    cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);
    
    if (!cpu->code_memory) {
        printf("No codes in the memory!\n");
        free(cpu);
        return NULL;
    }
    
    /* Make all stages busy except Fetch stage, initally to start the pipeline */
    for (int i = 1; i < NUM_STAGES; ++i)
    {
        CPU_Stage *stage = &cpu->stages[i];
        stage->stage_units[0].valid = 0;
        stage->stage_units[1].valid = 0;
    }
	
    for (int i = 0; i < ROB_SIZE; i++)
    {
        cpu->forwarding_reserve[i].value = 0;
        cpu->forwarding_reserve[i].valid = 0;
	
        cpu->forwarding[i].value = 0;
        cpu->forwarding[i].valid = 0;
    }
	for (int i = 0; i < ROB_SIZE; i++)
	{
		cpu->ROB[i].PC = -1;
		cpu->ROB[i].destination = -1;
		cpu->ROB[i].completed= COMPLETED;
		cpu->ROB[i].result = -1;
	}
	cpu->ROB_avail = ROB_SIZE;

    cpu->completed_instructions =0;
    cpu->data_hazards    =0;
    cpu->control_hazards =0;
    cpu->fetch_stalls    =0;
    cpu->full_reservation_count = 0;
    cpu->full_reorder_count = 0;
    
    init_globals(cpu);
    return cpu;
}

/*
 * This function de-allocates CPU cpu.
 *
 * Note : You are free to edit this function according to your
 *                 implementation
 */
void
CPU_stop(CPU* cpu)
{
	free(cpu->reserve_buffer->unit_list);
	free(cpu->reserve_buffer);
    free(cpu->code_memory);
    free(cpu);
}

/* Converts the PC(0000 series) into
 * array index for code memory
 *
 * Note : You are not supposed to edit this function
 *
 */
int
get_code_index(int pc);

int
get_code_index(int pc)
{
    return (pc - 0) / 4;
}

CPU_Instruction* 
get_cur_instruction(CPU* cpu)
{
    int code_index = get_code_index(cpu->pc);
    if (code_index>=cpu->code_memory_size) {
        return 0;
    }

    return &cpu->code_memory[code_index];
}

bool is_branch(char *opcode)
{
    if (strcmp(opcode, "bez") == 0||strcmp(opcode, "bgez") == 0||strcmp(opcode, "blez") == 0||strcmp(opcode, "bgtz") == 0||strcmp(opcode, "bltz") == 0){
        return true;
    }
    return false;
}

bool is_memory(char *opcode)
{
    if (strcmp(opcode, "ld") == 0 || strcmp(opcode, "st") == 0) {
        return true;
    }
    return false;
}

void update_reservation_station (int tag, int value, CPU *cpu)
{
    for(int i = 0; i < cpu->reserve_buffer->index; i++){
        Stage_Unit *unit = &cpu->reserve_buffer->unit_list[i];
        bool isBranch = strcmp(unit->opcode, "bez") == 0 
			|| strcmp(unit->opcode, "bgez") == 0 
			|| strcmp(unit->opcode, "blez") == 0
			|| strcmp(unit->opcode, "bgtz") == 0
			|| strcmp(unit->opcode, "bltz") == 0;
        bool isStore = strcmp(unit->opcode, "st") == 0;

		if (unit->rs1 >= 0 && unit->rs1 < ROB_SIZE && !unit->find_rs1){
			if (unit->rs1 == tag){
				unit->rs1_value= value;
				unit->find_rs1 = 1;
			}
		}
		if (unit->rs2 >= 0 && unit->rs2 < ROB_SIZE && !unit->find_rs2){
			if (unit->rs2 == tag){
				unit->rs2_value= value;
				unit->find_rs2 = 1;
			}
		}
        if (unit->rd >= 0 && unit->rd < ROB_SIZE){
            if (isBranch||isStore){
                if (!unit->find_rd){
					if (unit->rd == tag){
						unit->rd_value= value;
						unit->find_rd = 1;
					}
                }
            }
        }
    }
}


CPU_Stage *get_target_stage(int stage_index, CPU *cpu, bool reset_unit_length)
{
    CPU_Stage *stage = &cpu->stages[stage_index];
    if (reset_unit_length)
    {
        stage->unit_length = 0;
    }

    return stage;
}

void get_p_trace(char *p_trace, Stage_Unit *unit){
    char rd[12]={0};
    char rs1[12]={0};
    char rs2[12]={0};
    char imm[12]={0};

	if (unit->rd > -1)
	{
		bool isBranch = strcmp(unit->opcode, "bez") == 0
			|| strcmp(unit->opcode, "bgez") == 0 
			|| strcmp(unit->opcode, "blez") == 0 
			|| strcmp(unit->opcode, "bgtz") == 0 
			|| strcmp(unit->opcode, "bltz") == 0;
		bool isStore = strcmp(unit->opcode, "st") == 0;

		if (isBranch||isStore){
			if (unit->find_rd)
				sprintf(rd, "%d", unit->rd_value);
			else 
				sprintf(rd, "R%d", unit->rd);
		}
		else {
			sprintf(rd, "R%d", unit->rd);
		}
	}
	if (unit->rs1 > -1)
	{
		if (unit->find_rs1)
			sprintf(rs1, "%d", unit->rs1_value);
		else 
			sprintf(rs1, "R%d", unit->rs1);
	}
	if (unit->rs2 > -1)
	{
		if (unit->find_rs2)
			sprintf(rs2, "%d", unit->rs2_value);
		else 
			sprintf(rs2, "R%d", unit->rs2);
	}	

	if (unit->is_imm)
	{
		sprintf(imm, "#%d",unit->imm);
	}

	char pc[10]={0};
	sprintf(pc, "%4d",unit->pc);
	for(int i=0;i<10;++i)
	if(pc[i]==' ')
		pc[i]='0';

	sprintf(p_trace, "%s: %s %s %s %s imm(%s)",pc,unit->opcode,rd,rs1,rs2,imm);
}

void get_r_trace(char *p_trace, Stage_Unit *unit){
    char rd[12]={0};
    char rs1[12]={0};
    char rs2[12]={0};
    char imm[12]={0};

	if (unit->rd > -1)
	{
		bool isBranch = strcmp(unit->opcode, "bez") == 0 
			|| strcmp(unit->opcode, "bgez") == 0 
			|| strcmp(unit->opcode, "blez") == 0
			|| strcmp(unit->opcode, "bgtz") == 0
			|| strcmp(unit->opcode, "bltz") == 0;
		bool isStore = strcmp(unit->opcode, "st") == 0;

		if (isBranch||isStore){
			if (unit->find_rd)
				sprintf(rd, "%d", unit->rd_value);
			else 
				sprintf(rd, "ROB%d", unit->rd);
		}
		else {
			sprintf(rd, "ROB%d", unit->rd);
		}
	}
	if (unit->rs1 > -1)
	{
		if (unit->find_rs1)
			sprintf(rs1, "%d", unit->rs1_value);
		else 
			sprintf(rs1, "ROB%d", unit->rs1);
	}
	if (unit->rs2 > -1)
	{
		if (unit->find_rs2)
			sprintf(rs2, "%d", unit->rs2_value);
		else 
			sprintf(rs2, "ROB%d", unit->rs2);
	}	

	if (unit->is_imm)
	{
		sprintf(imm, "#%d",unit->imm);
	}

	char pc[10]={0};
	sprintf(pc, "%4d",unit->pc);
	for(int i=0;i<10;++i)
	if(pc[i]==' ')
		pc[i]='0';

	sprintf(p_trace, "%s: %s %s %s %s imm(%s)",pc,unit->opcode,rd,rs1,rs2,imm);
}

/* Debug function which dumps the cpu stage
 * content
 *
 * Note : You are not supposed to edit this function
 *
 */
void
print_stage_content(int stage, int p_index, Stage_Unit *unit, char *ending, FILE *f)
{

    char *trace = unit?((unit->valid) ? unit->trace:""):"";
    char p_trace[200] = {0};

    char *name_list[] = {"IF","ID","EXa","EXb1","EXb2","MEM1","MEM2","MEM3","MEM4","WB"};
    char *name = name_list[stage];

    if (stage==0)
    {
        strcpy(p_trace, trace);
    }
    else
    {
        if (unit->valid)
        {
        	if (unit->renamed){
	            get_r_trace(p_trace, unit);
        	}
			else {
				get_p_trace(p_trace, unit);
			}
        }
    }

    printLog(f, "| %-5s(P%d): %-32s%s", name, p_index+1, p_trace, ending);
}


void
print_WB_content(int stage, int p_index, ROB_entry_t *rob_e, char *ending, FILE *f)
{
    char p_trace[200] = {0};

	if (rob_e->completed && rob_e->PC >= 0){
		bool isBranch = strcmp(rob_e->opcode, "bez") == 0 
			|| strcmp(rob_e->opcode, "bgez") == 0 
			|| strcmp(rob_e->opcode, "blez") == 0
			|| strcmp(rob_e->opcode, "bgtz") == 0
			|| strcmp(rob_e->opcode, "bltz") == 0;
		bool isStore = strcmp(rob_e->opcode, "st") == 0;

		char pc[10]={0};
		sprintf(pc, "%4d",rob_e->PC);
		for(int i=0;i<10;++i)
		if(pc[i]==' ')
			pc[i]='0';

		if (isBranch||isStore){
			sprintf(p_trace, "%s: update rd %s ", pc, "No register update");
			printLog(f, "| %-5s(P%d): %-32s%s", "WB", p_index+1, p_trace, ending);
		}
		else {
			char rd[12]={0};
			if (rob_e->destination > -1)
			{
				sprintf(rd, "R%d(%d)", rob_e->destination, rob_e->result);
			}
			sprintf(p_trace, "%s: update rd %s ", pc, rd);
			printLog(f, "| %-5s(P%d): %-32s%s", "WB", p_index+1, p_trace, ending);
		}
	}
	else {
		printLog(f, "| %-5s(P%d): %-32s%s", "WB", p_index+1, "", ending);
	}
}

int issue(CPU *cpu)
{
    CPU_Stage *exa_stage = get_target_stage(EXa, cpu, true);
    CPU_Stage *exb_stage = get_target_stage(EXb, cpu, true);
    CPU_Stage *mem_stage = get_target_stage(MEM, cpu, true);

    printLog(cpu->f, "| IS  ");

    for(int i = 0; i < cpu->reserve_buffer->index; ){
        Stage_Unit *unit = &cpu->reserve_buffer->unit_list[i];

        int data_hazard = 0;
        int structural_hazard = 0;
        int control_hazard = 0;
        
        bool isBranch = strcmp(unit->opcode, "bez") == 0 
			|| strcmp(unit->opcode, "bgez") == 0 
			|| strcmp(unit->opcode, "blez") == 0
			|| strcmp(unit->opcode, "bgtz") == 0
			|| strcmp(unit->opcode, "bltz") == 0;
        bool isStore = strcmp(unit->opcode, "st") == 0;

	    if (unit->rs1>-1 && !unit->find_rs1 && cpu->forwarding[unit->rs1].valid == 1)
	    {
	        unit->find_rs1 = 1;
	        unit->rs1_value = cpu->forwarding[unit->rs1].value;
	    }
	    if (unit->rs2>-1 && !unit->find_rs2 && cpu->forwarding[unit->rs2].valid == 1)
	    {
	        unit->find_rs2 = 1;
	        unit->rs2_value = cpu->forwarding[unit->rs2].value;
	    }
	    if (unit->rd>-1 && !unit->find_rd && cpu->forwarding[unit->rd].valid == 1)
	    {
	        if ((isBranch || isStore))
	        {
	            unit->find_rd = 1;
	            unit->rd_value = cpu->forwarding[unit->rd].value;
	        }else
	        {
	            cpu->forwarding[unit->rd].valid = -1;
	        }
	        
	    }		

		if (unit->rs1 >= 0 && unit->rs1 < ROB_SIZE && !unit->find_rs1){
			data_hazard = 1;
		}
		if (unit->rs2 >= 0 && unit->rs2 < ROB_SIZE && !unit->find_rs2){
			data_hazard = 1;
		}
        if (unit->rd >= 0 && unit->rd < ROB_SIZE){
            if (isBranch||isStore){
                if (!unit->find_rd){
                    data_hazard = 1;
                }
            }
        }

        if (unit->action_type == ActionEXa && exa_stage->unit_length > 1){
            structural_hazard = 1;
        }
        if (unit->action_type == ActionEXb && exb_stage->unit_length > 0){
            structural_hazard = 1;
        }
        if (unit->action_type == ActionMemory && mem_stage->unit_length > 0){
            structural_hazard = 1;
        }

        // Copy data from dispatch latch to execute latch
        if (data_hazard == 0 && control_hazard == 0 && structural_hazard == 0)
        {
            char p_trace[100]={0};
            get_r_trace(p_trace, unit);
            char *tar_str = "EXa";

            if (unit->action_type == ActionEXa)  {
                exa_stage->stage_units[exa_stage->unit_length++] = *unit;
                tar_str = "EXa";
            }
            if (unit->action_type == ActionEXb) {
                exb_stage->stage_units[exb_stage->unit_length++] = *unit;
                tar_str = "EXb 1";
            }
            if (unit->action_type == ActionMemory) {
                mem_stage->stage_units[mem_stage->unit_length++] = *unit;
                tar_str = "MEM 1";
            }

            printLog(cpu->f, "%s => %s|\t", p_trace, tar_str);
            remove_at(i, cpu->reserve_buffer);
        }
        else
        {
            i++;            
        }
    }
    printLog(cpu->f,"\n");
}

/*
 *  Execute Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int execute_a(CPU *cpu, int times)
{
    CPU_Stage *stage = &cpu->stages[EXa + times - 1];
    if (times != 1) cpu->stages[EXa + times].unit_length = 0;

    for (int j = 0; j < 2; j++)
    {
        Stage_Unit *unit = &stage->stage_units[j];
        if (unit->valid)
        {
            print_stage_content(EXa, j, unit, "\t", cpu->f);

            if (times == 1)
            {
                bool is_exa = false;
                /* MOVC */
                if (strcmp(unit->opcode, "set") == 0)
                {
                    unit->buffer = unit->rs1_value; 
                    is_exa = true;   
                }

                if (strcmp(unit->opcode, "add") == 0)
                {
                    unit->buffer = unit->rs1_value + unit->rs2_value; 
                    is_exa = true; 
                }

                if (strcmp(unit->opcode, "sub") == 0)
                {
                    unit->buffer = unit->rs1_value - unit->rs2_value; 
                    is_exa = true; 
                }

                if (is_exa)
                {
                    cpu->forwarding_reserve[unit->rob_id].value = unit->buffer;
                    cpu->forwarding_reserve[unit->rob_id].valid = 1;

                }

				if (strcmp(unit->opcode, "bez") == 0 
				|| strcmp(unit->opcode, "bgez") == 0 
				|| strcmp(unit->opcode, "blez") == 0
				|| strcmp(unit->opcode, "bgtz") == 0
				|| strcmp(unit->opcode, "bltz") == 0){
					ROB_entry_t* rob = &cpu->ROB[unit->rob_id];
					rob->completed = true;
				}

				if (strcmp(unit->opcode, "ret") == 0){
					ROB_entry_t* rob = &cpu->ROB[unit->rob_id];
					rob->completed = true;
				}
            }
            else
            {
                CPU_Stage *t_stage = &cpu->stages[EXa+times];
                t_stage->stage_units[t_stage->unit_length++] = *unit;
            }
        
            unit->valid = 0;
        }
        else
        {
            print_stage_content(EXa, j, unit, "\t", cpu->f);
        }
    }

    return 0;
}

int execute_b(CPU *cpu, int times)
{
    CPU_Stage *stage = &cpu->stages[EXb + times - 1];
    if (times != 2)
        cpu->stages[EXb + times].unit_length = 0;

    for (int j = 0; j < 1; j++)
    {
        Stage_Unit *unit = &stage->stage_units[j];
        if (unit->valid)
        {
            if (times == 2)
            {
                bool is_exb = false;
                if (strcmp(unit->opcode, "mul") == 0)
                {
                    unit->buffer = unit->rs1_value * unit->rs2_value;
                    is_exb = true;
                }
                if (strcmp(unit->opcode, "div") == 0)
                {
                    unit->buffer = unit->rs1_value / unit->rs2_value;
                    is_exb = true;
                }
                if (is_exb)
                {
                    cpu->forwarding_reserve[unit->rob_id].value = unit->buffer;
                    cpu->forwarding_reserve[unit->rob_id].valid = 1;

                }   
            }
			else
            {
                CPU_Stage *t_stage = &cpu->stages[EXb + times];
                t_stage->stage_units[t_stage->unit_length++] = *unit;
            }
            
            print_stage_content(EXb+times-1, j, unit, "\t", cpu->f);
            unit->valid = 0;
        }
        else
        {
            print_stage_content(EXb+times-1, j, unit, "\t", cpu->f);
        }
    }

    return 0;
}

/*
 *  Memory Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int memory(CPU *cpu, int times)
{
    CPU_Stage *stage = &cpu->stages[MEM + times - 1];
    if (times != 4)
        cpu->stages[MEM + times].unit_length = 0;

    for (int j = 0; j < 1; j++)
    {
        Stage_Unit *unit = &stage->stage_units[j];
        if (unit->valid)
        {
            print_stage_content(MEM+times-1, j, unit, "|\n", cpu->f);

            if (times == 4)
            {
                /* LOAD */
                if (strcmp(unit->opcode, "ld") == 0)
                {
                    unit->buffer = cpu->data_memory[unit->rs1_value / 4];
                    cpu->forwarding_reserve[unit->rob_id].value = unit->buffer;
                    cpu->forwarding_reserve[unit->rob_id].valid = 1;

                }
				/* STORE */
				else {				
					cpu->data_memory[unit->rs1_value / 4] = unit->rd_value;

					ROB_entry_t* rob = &cpu->ROB[unit->rob_id];
					rob->completed = true;
				}
            }
            else
            {
                CPU_Stage *t_stage = &cpu->stages[MEM + times];
                t_stage->stage_units[t_stage->unit_length++] = *unit;
            }

            unit->valid = 0;
        }
        else
        {
            print_stage_content(MEM+times-1, j, unit, "|\n", cpu->f);
        }
    }

    return 0;
}

/*
 *  Writeback Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int writeback(CPU *cpu)
{
    for (int j = 0; j < 2; j++){
		ROB_entry_t* rob = &cpu->ROB[cpu->ROB_head];

        if (rob->completed && rob->PC >= 0){
            print_WB_content(WB, j, rob, j == 0 ? "\t" : "\n", cpu->f);
            if (rob->destination >= 0 && rob->destination < ROB_SIZE){
				if (rob->destination >=0){
					Arc_Reg_t* reg = &cpu->registers[rob->destination];
					if (reg->tag == cpu->ROB_head){
						reg->value = rob->result;
						reg->status= RegStatus_VALID;
						reg->tag = -1;
					}
				}
            }

			cpu->ROB_head++;
			cpu->ROB_avail++;
			if (cpu->ROB_head >= ROB_SIZE)
				cpu->ROB_head = 0;
			
			rob->completed	 = 0;
			rob->destination = -1;
			rob->exception	 = 0;
			rob->result 	 = -1;
			
            cpu->ins_completed = get_code_index(rob->PC);
            cpu->completed_instructions++;

            if (strcmp(rob->opcode, "ret") == 0){
                cpu->ins_completed = cpu->code_memory_size;
                cpu->completed = true;
            }
        }
		else {
			print_WB_content(WB, j, rob, j == 0 ? "\t" : "\n", cpu->f);
		}
    }

    return 0;
}

void print_display(CPU *cpu)
{
    FILE *f = cpu->f;

    int count = 0;

    printLog(f, "------------ Reserve Station ----------\n");
    for (int i = 0; i < cpu->reserve_buffer->index; i++)
    {
        Stage_Unit *unit = &cpu->reserve_buffer->unit_list[i];
        char p_trace[100] = {0};
        get_r_trace(p_trace, unit);
        printLog(f, "|%s\n", p_trace);
    }

    printLog(f, "\n------------ Reorder Buffer----------\n");
	for (int i = 0; i < ROB_SIZE; i++)
	{
		if (i == cpu->ROB_head && i == cpu->ROB_tail)
			printLog(f, "| ROB%d [dest: %d, result: %d, (e: %d, completed: %d)] <- head & tail\n", i, cpu->ROB[i].destination, cpu->ROB[i].result, cpu->ROB[i].exception, cpu->ROB[i].completed);			
		else if (i == cpu->ROB_head)
			printLog(f, "| ROB%d [dest: %d, result: %d, (e: %d, completed: %d)] <- head\n", i, cpu->ROB[i].destination, cpu->ROB[i].result, cpu->ROB[i].exception, cpu->ROB[i].completed);
		else if (i == cpu->ROB_tail)
			printLog(f, "| ROB%d [dest: %d, result: %d, (e: %d, completed: %d)] <- tail\n", i, cpu->ROB[i].destination, cpu->ROB[i].result, cpu->ROB[i].exception, cpu->ROB[i].completed);			
		else
			printLog(f, "| ROB%d [dest: %d, result: %d, (e: %d, completed: %d)]\n", i, cpu->ROB[i].destination, cpu->ROB[i].result, cpu->ROB[i].exception, cpu->ROB[i].completed);
	}


    printLog(f, "\n------------ STATE OF ARCHITECTURAL REGISTER FILE ----------\n");
	printLog(f, "R# [(status 0=invalid, 1=valid), tag, value] \n");
	for (int reg = 0; reg < REG_SIZE; reg++)
	{
		printLog(f, "| R%d [(%d) %d, %d] ", reg, cpu->registers[reg].status, cpu->registers[reg].tag, cpu->registers[reg].value);
		if (reg%4 == 3) printLog(f, "|\n");
	}
    printLog(f, "--------------------------------------------------------------------------------\n\n");
}

void print_memory(CPU *cpu)
{

    char *log_path = malloc(100);
    strcpy(log_path, cpu->log_path);
    char *mem_path = replaceWord(log_path, ".log", "_memory.log");

    FILE *f;
    f = fopen(mem_path, "w");
    if (f == NULL)
    {
        printf("Open memory log file Error!");
        return;
    }
    for (int mem = 0; mem < MEMORY_SIZE; mem++)
    {
        fprintf(f, "|    MEM[%2d]    |    Value=%d    |\n", mem * 4, cpu->data_memory[mem]);
    }
}

void print_line(FILE *f)
{
    for (int t = 0; t < 38; t++)
    {
        printLog(f, " --- ");
    }
    printLog(f, " \n");
}

void print_spaces(FILE *f, int num)
{
    for (int t = 0; t < num; t++)
    {
        printLog(f, "|                                               ");
    }
}

/*
 *  CPU CPU simulation loop
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int CPU_run(CPU *cpu)
{
    FILE *f = cpu->f;
    while (cpu->clock < cpu->clock_stop)
    {

        /* All the instructions committed, so exit */
        if (cpu->completed)
        {
            printLog(f, "(CPU) >> Simulation Complete\n");
            break;
        }
        if (cpu->clock + 1 == 224)
        {
            int a = 0;
        }

        printLog(f, "\n====================================================================\n");
        printLog(f, "Clock Cycle #: %d\n", cpu->clock + 1);

        writeback(cpu);
        print_line(cpu->f);

        cpu->stages[WB].unit_length = 0;
        if (cpu->ins_completed < cpu->code_memory_size)
        {
			for (int i=0; i<ROB_SIZE; i++)
			{
				cpu->forwarding[i].value = cpu->forwarding_reserve[i].value;
				cpu->forwarding[i].valid = cpu->forwarding_reserve[i].valid;
			
				if (cpu->forwarding_reserve[i].valid){
					ROB_entry_t* rob = &cpu->ROB[i];
					rob->completed = true;
					rob->result = cpu->forwarding_reserve[i].value;
				}

				cpu->forwarding_reserve[i].value = 0;
				cpu->forwarding_reserve[i].valid = 0;				
			}

            execute_a(cpu, 1);
            execute_b(cpu, 2);
            memory(cpu, 4);

            print_spaces(cpu->f, 2);

            execute_b(cpu, 1);
            memory(cpu, 3);

            print_spaces(cpu->f, 3);

            memory(cpu, 2);

            print_spaces(cpu->f, 3);

            memory(cpu, 1);

            print_line(cpu->f);
            issue(cpu);

            dispatch(cpu);

            fetch(cpu);
            print_line(cpu->f);

        }

        if (cpu->commandType == DISPLAY)
        {
            print_display(cpu);
        }

        cpu->clock++;
    }

    printf("Number of ID stage stalls due to the full reservation station: %d \n", cpu->full_reservation_count);
    printf("Number of ID stage stalls due to the full reorder buffer: %d \n", cpu->full_reorder_count);
    printf("Number of fetch stalls: %d \n", cpu->fetch_stalls);
    printf("\n");
    printf("Total execution cycles: %d\n", cpu->clock);
    printf("IPC: %f\n", ((double)cpu->completed_instructions/cpu->clock));

    print_memory(cpu);

    return 0;
}

Unit_Buffer *create_unit_buffer(int unit_size)
{
    Unit_Buffer *unit_buffer = malloc(sizeof(Unit_Buffer));
    if (!unit_buffer){
        return NULL;
    }
    unit_buffer->index = 0;
    unit_buffer->length = unit_size;
    unit_buffer->unit_list = malloc(sizeof(Stage_Unit)*unit_size); 
    
    return unit_buffer;
}
int append_unit(Stage_Unit unit, Unit_Buffer *buffer)
{
    if (buffer->index >= buffer->length){
        return -1;
    }
    buffer->unit_list[buffer->index] = unit;
    buffer->index++;
    return 0;
}


void remove_at(int idx, Unit_Buffer *buffer){
    buffer->index--;
    for (int i = idx; i < buffer->index; i++)
    {
        buffer->unit_list[i] = buffer->unit_list[i+1];
    }
}

void empty_buffer(Unit_Buffer *buffer){
    buffer->index = 0;
}



CPU_Instruction* create_code_memory(const char* filename, int* size)
{
    if (!filename) {
        return NULL;
    }
    
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    int code_memory_size = 0;
    
    while ((nread = getline(&line, &len, fp)) != -1) {
        code_memory_size++;
    }
    *size = code_memory_size;
    if (!code_memory_size) {
        fclose(fp);
        return NULL;
    }
    
    CPU_Instruction* code_memory =
    malloc(sizeof(struct CPU_Instruction) * code_memory_size);
    if (!code_memory) {
        fclose(fp);
        return NULL;
    }

    rewind(fp);
    int current_instruction = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
        create_CPU_instruction(&code_memory[current_instruction], line);
        current_instruction++;
    }
    
    free(line);
    fclose(fp);
    return code_memory;
}

void count_one(int *count){
    *count = *count + 1;
}

