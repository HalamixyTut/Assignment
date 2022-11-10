//
//  cpu.c
//  Project1
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
    
    CPU* cpu = malloc(sizeof(*cpu));
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
    cpu->completed_instructions =0;
    cpu->pc = 0;
    cpu->clock_stop = 80000;
    cpu->read_count = 0;
    cpu->up_date_reg = -1;
    // value; count; read_count; write_count; is_writing;
    
    memset(cpu->stage, 0, sizeof(CPU_Stage) * NUM_STAGES);
    memset(cpu->data_memory, 0, sizeof(int) * MEMORY_SIZE);
    load_memory_map(cpu->data_memory, "memory_map.txt");
    
    /* Parse input file and create code memory */
    cpu->regs= create_registers(16);
    cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);
    
    if (!cpu->code_memory) {
        printf("No codes in the memory!\n");
        free(cpu);
        return NULL;
    }
    
    /* Make all stages busy except Fetch stage, initally to start the pipeline */
//    for (int i = 1; i < NUM_STAGES; ++i) {
//        cpu->stage[i].busy = 0;
//    }
//    cpu->stage[0].valid = 1;
    
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

bool is_branch(char *opcode){
    if (strcmp(opcode, "bez") == 0||strcmp(opcode, "bgez") == 0||strcmp(opcode, "blez") == 0||strcmp(opcode, "bgtz") == 0||strcmp(opcode, "bltz") == 0){
        return true;
    }
    return false;
}


/* Debug function which dumps the cpu stage
 * content
 *
 * Note : You are not supposed to edit this function
 *
 */
void
print_stage_content(char* name, CPU_Stage* stage, FILE *f)
{
    printLog(f, "%-15s: %s", name, stage->trace);
    printLog(f, "\n");
}

/*
 *  Execute Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
execute_a(CPU* cpu, int times)
{
    CPU_Stage* stage = &cpu->stage[EXa];

    if ((stage->valid) && (!stage->stalled)) {
        
        /* Copy data from Execute latch to Memory latch*/
        print_stage_content("EXa", stage, cpu->f);
        
        if (times == 1) {
            /* Store */
            if (strcmp(stage->opcode, "st") == 0) {
                stage->buffer = stage->rs1_value;
            }
            
            /* LOAD */
            if (strcmp(stage->opcode, "ld") == 0) {
                stage->buffer = stage->rs1_value;
            }
            /* MOVC */
            if (strcmp(stage->opcode, "set") == 0) {
                stage->buffer = stage->rs1_value;
            }
            if (is_branch(stage->opcode)) {
                stage->buffer = stage->buffer;
            }
            
            if (strcmp(stage->opcode, "add") == 0) {
                stage->buffer = stage->rs1_value + stage->rs2_value;
            }
            
            if (strcmp(stage->opcode, "sub") == 0) {
                stage->buffer = stage->rs1_value - stage->rs2_value;
            }
            if (is_branch(stage->opcode)) {
                stage->buffer = stage->imm;
            }
            
            
            if (strcmp(stage->opcode, "ret") == 0) {
            }
            cpu->stage[EXb] = cpu->stage[EXa];
            stage->valid = 0;
        }else{
            execute_a(cpu, times-1);
        }
        
    }
    
    return 0;
}

int
execute_b(CPU* cpu, int times)
{
    CPU_Stage* stage = &cpu->stage[EXb+times-1];
    if ((stage->valid) && (!stage->stalled)) {
        
        /* Copy data from Execute latch to Memory latch*/
        
        char bu[20] = {0};
        sprintf(bu, "Exb %d", times);
        print_stage_content(bu, stage, cpu->f);
        if (times == 2) {
            if (strcmp(stage->opcode, "mul") == 0 ) {
                stage->buffer = stage->rs1_value * stage->rs2_value;
            }
            if (strcmp(stage->opcode, "div") == 0 ) {
                stage->buffer = stage->rs1_value / stage->rs2_value;
            }
            
            if (is_branch(stage->opcode)) {
                
                int z = stage->rd_value;
                cpu->branch = NotTaken;
                if (strcmp(stage->opcode, "bez") == 0 && z==0) {
                    cpu->branch = Taken;
                }
                if (strcmp(stage->opcode, "bgez") == 0 && z>=0) {
                    cpu->branch = Taken;
                }
                if (strcmp(stage->opcode, "blez") == 0 && z<=0) {
                    cpu->branch = Taken;
                }
                if (strcmp(stage->opcode, "bgtz") == 0 && z>0) {
                    cpu->branch = Taken;
                }
                if (strcmp(stage->opcode, "bltz") == 0 && z<0) {
                    cpu->branch = Taken;
                }
                if (cpu->branch == Taken) {
                    cpu->pc = stage->buffer;
                }
                if (cpu->branch == NotTaken||cpu->branch == Taken) {
                    cpu->branch = NotWaiting;
                }
            }
            
            if (strcmp(stage->opcode, "ret") == 0) {
            }        
        }
        cpu->stage[EXb+times] = *stage;
        stage->valid = 0;
    }
    if(times > 1){
            execute_b(cpu, times-1);
        }
    return 0;
}


/*
 *  Memory Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
memory(CPU* cpu, int times)
{
    CPU_Stage* stage = &cpu->stage[MEM+times-1];
    if ((stage->valid) && !stage->stalled) {
        char bu[20] = {0};
        sprintf(bu, "Memory %d", times);
        print_stage_content(bu, stage, cpu->f);
        if (times == 4) {
            /* Store */
            if (strcmp(stage->opcode, "st") == 0) {
                cpu->data_memory[stage->buffer/4] = stage->rd_value;
            }
            
            /* LOAD */
            if (strcmp(stage->opcode, "ld") == 0) {
                stage->buffer = cpu->data_memory[stage->buffer/4];
            }
        }
        cpu->stage[MEM+times] = *stage;
        stage->valid = 0;
    }
    if (times>1)
    {
        memory(cpu, times-1);
    }
    return 0;
}

/*
 *  Writeback Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
writeback(CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[WB];
    if (stage->valid) {
        if (is_branch(stage->opcode) || strcmp(stage->opcode, "st") == 0 ) {
        }else{
            if (stage->rd >=0 && stage->rd <16) {
                cpu->up_date_reg = stage->rd;
                cpu->regs[stage->rd].value = stage->buffer;
            }
        }

        cpu->ins_completed = get_code_index(stage->pc);
        stage->valid = 0;
        cpu->completed_instructions++;
        
        if (strcmp(stage->opcode, "ret") == 0) {
            cpu->ins_completed = cpu->code_memory_size;
        }
        
        print_stage_content("Writeback", stage, cpu->f);
    }
    return 0;
}

void print_display(CPU *cpu){
    FILE *f = cpu->f;
    printLog(f, "=============== STATE OF ARCHITECTURAL REGISTER FILE ==========\n\n");
    for (int reg=0; reg<16; reg++) {
        char *status = cpu->regs[reg].is_writing==true?"writing":"valid";
        printLog(f, "|   REG[%2d]   |   Value=%d   |   Status=%s   |\n",reg,cpu->regs[reg],status);
    }
    
//    printf("\n============== STATE OF DATA MEMORY =============\n\n");
//    for (int mem=1000; mem<1004; mem++) {
//        printf("|    MEM[%2d]    |    Value=%d    |\n",mem,cpu->data_memory[mem]);
//    }
}

void print_memory(CPU *cpu){
    
    char*log_path = malloc(100);
    strcpy(log_path, cpu->log_path);
    char *mem_path = replaceWord(log_path,".log","_memory_map.txt");
    
    FILE *f;
    f = fopen(mem_path, "w");
    if (f == NULL) {
        printf("Open memory log file Error!");
        return;
    }
    fprintf(f, "\n============== STATE OF DATA MEMORY =============\n\n");
    for (int mem=0; mem<MEMORY_SIZE; mem++) {
        fprintf(f, "|    MEM[%2d]    |    Value=%d    |\n",mem*4,cpu->data_memory[mem]);
    }
}

/*
 *  CPU CPU simulation loop
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
CPU_run(CPU* cpu)
{
    FILE *f = cpu->f;
    while (cpu->clock < cpu->clock_stop) {
        
        /* All the instructions committed, so exit */
        if (cpu->ins_completed == cpu->code_memory_size) {
//            printLog(f, "(CPU) >> Simulation Complete\n");
            printLog(f, "================================\n");
            printLog(f, "\n");
            break;
        }
        
        printLog(f, "================================\n");
        printLog(f, "Clock Cycle #: %d\n", cpu->clock+1);
        printLog(f, "--------------------------------\n");
        
        
        writeback(cpu);
        memory(cpu, 4);
        execute_b(cpu, 2);
        execute_a(cpu, 1);
        decode(cpu);
        fetch(cpu);
        
        if (cpu->up_date_reg != -1) {
            cpu->regs[cpu->up_date_reg].is_writing = false;
            cpu->up_date_reg = -1;
        }
        
        if (cpu->commandType == DISPLAY) {
            print_display(cpu);
        }
        
        cpu->clock++;
    }
    
    printf("Stalled cycles due to data hazard: %d \n", cpu->data_hazard_count);
    printf("Stalled cycles due to control hazard: %d \n", cpu->control_hazard_count);
    printf("Stalled cycles due to structural hazard: %d \n", cpu->structural_hazard_count);
    printf("\n");
    printf("Total stalls: %d \n", cpu->data_hazard_count+cpu->control_hazard_count+cpu->structural_hazard_count);
    printf("Total execution cycles: %d\n", cpu->clock);
    printf("IPC: %f\n", ((double)cpu->completed_instructions/cpu->clock));

    print_memory(cpu);

    return 0;
}

Register* create_registers(int size){
    Register* regs = malloc(sizeof(*regs) * size);
    if (!regs) {
        return NULL;
    }
    for (int i=0; i<size; i++){
        regs[i].value = 0;
        regs[i].is_writing = false;
    }
    return regs;
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
    malloc(sizeof(*code_memory) * code_memory_size);
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
