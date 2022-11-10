//
//  parse_trace.c
//  Project1
//
//  Created by Xiang on 2020/1/28.
//  Copyright © 2020年 Xiang. All rights reserved.
//

#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_num_from_string(char* buffer){
    char str[16];
    int j = 0;
    for (int i = 1; buffer[i] != '\0'; ++i) {
        str[j] = buffer[i];
        j++;
    }
    str[j] = '\0';
    return atoi(str);
}

void create_CPU_instruction(CPU_Instruction* ins, char* buffer)
{
    
    strcpy(ins->trace, buffer);
    if (ins->trace[strlen(ins->trace)-1] == '\n') {
        ins->trace[strlen(ins->trace)-1] = '\0';
    }
    
    char* token = strtok(buffer, " ");
    int token_num = 0;
    char tokens[6][128];
    while (token != NULL) {
        if (token_num>0) {
            strcpy(tokens[token_num-1], token);
        }
        token_num++;
        token = strtok(NULL, ", ");
    }
    token_num--;
    
    
    ins->rd = -1;
    ins->rs1 = -1;
    ins->rs2 = -1;
    ins->imm = -1;
    ins->is_imm = false;

    char *token0 = tokens[0];
    if (token0[strlen(token0)-1] == '\n') {
        token0[strlen(token0)-1] = '\0';
    }
    if (token0[strlen(token0)-1] == '\r') {
        token0[strlen(token0)-1] = '\0';
    }
    strcpy(ins->opcode, token0);
    char *token_last = tokens[token_num-1];
    if (token_last[0] == '#') {
        ins->is_imm = true;
    }
    
    
    if (strcmp(ins->opcode, "set") == 0) { // set
        ins->rd = get_num_from_string(tokens[1]);
        ins->imm = get_num_from_string(tokens[2]);
    }
    
    if (strcmp(ins->opcode, "ld") == 0) { // load
        ins->rd = get_num_from_string(tokens[1]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[2]);
        }else{
            ins->rs1 = get_num_from_string(tokens[2]);
        }
    }
    
    if (strcmp(ins->opcode, "st") == 0) {// store
        ins->rd = get_num_from_string(tokens[1]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[2]);
        }else{
            ins->rs1 = get_num_from_string(tokens[2]);
        }
    }
    
    if (strcmp(ins->opcode, "add") == 0) {
        ins->rd = get_num_from_string(tokens[1]);
        ins->rs1 = get_num_from_string(tokens[2]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[3]);
        }else{
            ins->rs2 = get_num_from_string(tokens[3]);
        }
    }
    
    if (strcmp(ins->opcode, "sub") == 0) {
        ins->rd = get_num_from_string(tokens[1]);
        ins->rs1 = get_num_from_string(tokens[2]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[3]);
        }else{
            ins->rs2 = get_num_from_string(tokens[3]);
        }
    }
    
    if (strcmp(ins->opcode, "mul") == 0) {
        ins->rd = get_num_from_string(tokens[1]);
        ins->rs1 = get_num_from_string(tokens[2]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[3]);
        }else{
            ins->rs2 = get_num_from_string(tokens[3]);
        }
    }
    if (strcmp(ins->opcode, "div") == 0) {
        ins->rd = get_num_from_string(tokens[1]);
        ins->rs1 = get_num_from_string(tokens[2]);
        if (ins->is_imm) {
            ins->imm = get_num_from_string(tokens[3]);
        }else{
            ins->rs2 = get_num_from_string(tokens[3]);
        }
    }
    
    if (strcmp(ins->opcode, "bez") == 0||strcmp(ins->opcode, "bgez") == 0||strcmp(ins->opcode, "blez") == 0||strcmp(ins->opcode, "bgtz") == 0||strcmp(ins->opcode, "bltz") == 0) {
        ins->rd = get_num_from_string(tokens[1]);
        ins->imm = get_num_from_string(tokens[2]);
    }
}

void print_Instruction(CPU_Instruction* ins){
//    char opcode[128];    // Operation Code
//    int rd;            // Destination Register Address
//    int rs1;            // Source-1 Register Address
//    int rs2;            // Source-2 Register Address
//    int imm;            // Literal Value
//    bool is_imm; // whether is imm
//    char trace[128];
    printf("==============Print Instruction ==================\n");
    printf("opcode: %s\n",ins->opcode);
    printf("rd: %d\n",ins->rd);
    printf("rs1: %d\n",ins->rs1);
    printf("rs2: %d\n",ins->rs2);
    printf("imm: %d\n",ins->imm);
    printf("is_imm: %s\n",ins->is_imm?"true":"false");
    printf("trace: %s\n",ins->trace);
    printf("==============End Instruction ====================\n");






}
