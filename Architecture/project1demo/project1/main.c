/*********************************************************************************
* 
*  Created by Xiang on 2020/3/9.
*  Copyright (c) 2020-2021 Xiang. All rights reserved.
* 
**********************************************************************************/
//
//  main.c
//  Pipeline
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"



void run_cpu_fun(char* file_path, CommandType type){
    CPU *cpu = CPU_init(file_path);
    if (!cpu) {
        fprintf(stderr, "Error : Unable to initialize CPU\n");
        exit(1);
    }
    cpu->commandType = type;
    printf("Start...\n");
    CPU_run(cpu);
    CPU_stop(cpu);
    printf("...End\n");
}

int main(int argc, const char * argv[]) {
    if (argc<=1) {
        fprintf(stderr, "Error : missing required args\n");
        return -1;
    }
    char* filename = (char*)argv[1];

    CommandType type = DISPLAY;
    
    run_cpu_fun(filename, type);
    
    return 0;
}
