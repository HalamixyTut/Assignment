#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cpu.h"
#include "tool.h"

// Global instruction id
static int dinst_id =0;

// initialize global variables 
void init_globals(CPU *cpu) {
    // Although it is not necessary, 
    // you are allowed to add extra variables into the CPU structure. 
    // Use this function when you need to initialize such variables. 
    // In such a case, you are allowed to submit your project.c with cpu.h. 

}

/*
 *  Fetch Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
fetch(CPU* cpu) {

    CPU_Stage* stage = &cpu->stages[F];
    CPU_Stage* next_stage = &cpu->stages[DRF];

    if (cpu->branch == Waiting)
        return 0;

    if (stage->stalled && !cpu->stages[DRF].stalled) {
        stage->stalled = 0;
        for (int i=0; i<2; i++)
        {
            Stage_Unit *unit = &stage->stage_units[i];
            if (unit->valid) {
                cpu->stages[DRF].stage_units[i] = cpu->stages[F].stage_units[i];
                print_stage_content(F, i, unit, i == 0 ? "\t" : "\n", cpu->f);
                unit->valid = 0;
            }
        }
        return 0;
    }
    // when a branch is conditional, 
    // the fetch stage needs to be stalled,
    // until the branch is resolved. 
    if (!stage->stalled) {
        for (int j = 0; j < 2; j++) {
            Stage_Unit *unit = &stage->stage_units[j];
            /* Fetch an instruction indexed by cpu->pc */
            CPU_Instruction* current_ins = get_cur_instruction(cpu);
            if (current_ins == NULL)
                continue;

            /* Fetch stage retrieve the current instruction's information. */
            unit->pc = cpu->pc ;
            strcpy(unit->opcode, current_ins->opcode);
            strcpy(unit->trace, current_ins->trace);
            unit->rd = current_ins->rd;
            unit->rs1 = current_ins->rs1;
            unit->rs2 = current_ins->rs2;
            unit->imm = current_ins->imm;
            unit->is_imm = current_ins->is_imm;
            unit->rd = current_ins->rd;
            unit->valid = 1;
            unit->renamed = 0;
            unit->inst_id = dinst_id;
            dinst_id++;
            if (dinst_id > MAX_INST_ID)
                dinst_id = 0;
            cpu->pc = cpu->pc + 4;
        }
    }

    if (cpu->stages[DRF].stalled) {
        stage->stalled = 1;
        cpu->fetch_stalls += 1;
    }
    else {
        for (int i=0; i<2; i++) {
            if (cpu->stages[F].stage_units[i].valid) {
                cpu->stages[DRF].stage_units[i] = cpu->stages[F].stage_units[i];
            }
        }
    }

    for (int i=0; i<2; i++) {
        Stage_Unit *unit = &stage->stage_units[i];
        /* Update PC for next instruction */
        print_stage_content(F, i, unit, i == 0 ? "\t" : "\n", cpu->f);
        if (!stage->stalled)
            unit->valid = 0;
    }
    return 0;
}

/*
 *  Decode Stage of CPU Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
dispatch(CPU* cpu) {
    CPU_Stage *stage = &cpu->stages[DRF];
    stage->stalled = 0;

    int reservation_station_full_flag = 0;
    int reorder_buffer_full_flag = 0;

    for (int j = 0; j < 2; j++) {
        int branch_true = 0;
        Stage_Unit *unit = &stage->stage_units[j];

        if (unit->valid) {
            // Tip 1. 
            // You must check the reservation station and ROB has a free entry 
            // The reservation statation is full if
            //      cpu->reserve_buffer->index < cpu->reserve_buffer->length
            // The ROB entry has no free space if
            //      cpu->ROB_avail > 0

            if (cpu->reserve_buffer->index == cpu->reserve_buffer->length) {
                stage->stalled = 1;
                reservation_station_full_flag = 1;
            }

            if (cpu->ROB_avail == 0) {
                stage->stalled = 1;
                reorder_buffer_full_flag = 1;
            }

            // Tip 2. Decode the instruction
            // 
            // Depending on the opcode, you need to specify the instruction's function in the pipeline. 
            // We set "ActionEXa" as default.
            // 
            // e.g., 
            //  ActionMemory is for memory instructions (ld/st)
            //  ActionEXb is for mul/div
            //  ActionEXa is for all other instructions including branch instruction . 
            //  Note. branch is completed at ID stage. The branch instruction just go through the EXa path. 
            //
            // 
            if (!stage->stalled) {
                if (unit->rs1 > -1) {
                    if (cpu->registers[unit->rs1].status) {
                        unit->rs1_value = cpu->registers[unit->rs1].value;
                        unit->find_rs1 = 1;
                    }
                    else {
                        unit->rs1 = cpu->registers[unit->rs1].tag;
                        if (cpu->ROB[unit->rs1].completed == COMPLETED) {
                            unit->rs1_value = cpu->ROB[unit->rs1].result;
                            unit->find_rs1 = 1;
                        }
                    }
                }
                else if (unit->is_imm) {
                    unit->rs1_value = unit->imm;
                    unit->find_rs1 = 1;
                }

                if (unit->rs2 > -1) {
                    if (cpu->registers[unit->rs2].status) {
                        unit->rs2_value = cpu->registers[unit->rs2].value;
                        unit->find_rs2 = 1;
                    }
                    else {
                        unit->rs2 = cpu->registers[unit->rs2].tag;
                        if (cpu->ROB[unit->rs2].completed == COMPLETED) {
                            unit->rs2_value = cpu->ROB[unit->rs2].result;
                            unit->find_rs2 = 1;
                        }
                    }
                }
                else if (unit->is_imm) {
                    unit->rs2_value = unit->imm;
                    unit->find_rs2 = 1;
                }

                if (is_branch(unit->opcode)) {
                    stage->stalled = 1;
                    if (!unit->renamed) {
                        unit->rd = cpu->registers[unit->rd].tag;
                    }

                    if (cpu->ROB[unit->rd].completed == COMPLETED) {
                        unit->find_rd = 1;
                        stage->stalled = 0;
                        unit->rd_value = cpu->ROB[unit->rd].result;
                        int z = unit->rd_value;
                        unit->rd = -1;
                        if (strcmp(unit->opcode, "bez") == 0 && z==0) {
                            branch_true = 1;
                        }
                        if (strcmp(unit->opcode, "bgez") == 0 && z>=0) {
                            branch_true = 1;
                        }
                        if (strcmp(unit->opcode, "blez") == 0 && z<=0) {
                            branch_true = 1;
                        }
                        if (strcmp(unit->opcode, "bgtz") == 0 && z>0) {
                            branch_true = 1;
                        }
                        if (strcmp(unit->opcode, "bltz") == 0 && z<0) {
                            branch_true = 1;
                        }
                        if (branch_true) {
                            cpu->stages[F].stage_units[0].valid = 0;
                            cpu->stages[F].stage_units[1].valid = 0;
                            if (j == 0)
                                cpu->stages[DRF].stage_units[1].valid = 0;
                            cpu->pc = unit->imm;
                        }
                    }
                }

                //else if (strcmp(unit->opcode, "st") != 0 || strcmp(unit->opcode, "ret") != 0)
                //    unit->action_type = ActionEXa;
                if (strcmp(unit->opcode, "mul") == 0||strcmp(unit->opcode, "div") == 0)
                    unit->action_type = ActionEXb;
                if (strcmp(unit->opcode, "ld") == 0||strcmp(unit->opcode, "st") == 0)
                    unit->action_type = ActionMemory;

                unit->renamed = 1;
            }

            // Tip 3. Register renaming with ROB
            // 
            // You need to allocate an ROB entry.
            // A cpu register needs to remember the ROB tag. 
            // 
            // First, the ID stage has to reserve an entry ROB.
            //        if an instruction has no destination, set the entry's desitnation '-1'.
            // Second, the ID stage has to reserve an entry in the reservation station.
            // Third, the ID stage has to read operands. 
            //
            // Read operands from three sources
            // 1. read an operand from registers
            // 2. read an operand from re-order buffers
            // 3. get an operand from forwardings
            //
            // If an operand has failed to achieve its operand value, 
            // the operand needs to be set to -1.             
            // 
            // ROB allocation codes are provided as below. 
            //
            if (!stage->stalled) {
                ROB_entry_t* rob = &cpu->ROB[cpu->ROB_tail];
                rob->completed = EXECUTING;
                rob->result = -1;
                rob->PC = unit->pc;
                strcpy(rob->opcode, unit->opcode);

                if (strcmp(unit->opcode, "st") != 0) {
                    rob->destination = unit->rd;
                    if (unit->rd != -1)
                        cpu->registers[unit->rd].tag = cpu->ROB_tail;
                } else {
                    rob->destination = -1;
                }
                if (unit->rd != -1)
                    cpu->registers[unit->rd].status = RegStatus_INVALID;
                unit->rob_id = cpu->ROB_tail;
                if (strcmp(unit->opcode, "ret") != 0) {
                    if (strcmp(unit->opcode, "st") != 0)
                        unit->rd = cpu->ROB_tail;
                    else
                        unit->rd = cpu->registers[unit->rd].tag;
                }

                cpu->ROB_avail--;
                cpu->ROB_tail++;
                if (cpu->ROB_tail >= ROB_SIZE)
                    cpu->ROB_tail = 0;

                // Tip 4. Dependency Analysis.
                // 
                // You need to analyze the dependencies between instructions. 
                // The dependency analysis is the same as what have you performed in the project #1. 
                // However, you do not need to stall the pipeline in the out-of-order pipeline. 
                //

                // Tip 5. Branch handling
                // 
                // The branch instruction must be solved at the ID stage. 
                // Once the branch is a taken branch, 
                // you need to squash instruction at IF stage. 
                // Otherwise, you can continue. 
                //

                // Return instruction is decoded means that the pipeline ends here. 
                // Note. The branch instruction is already solved at the ID stage. 
                // We stop fetching the instruction. 
                if (strcmp(unit->opcode, "ret") == 0 ) {
                    cpu->stages[F].stage_units[0].valid = 0;
                    cpu->stages[F].stage_units[1].valid = 0;
                    if (j == 0)
                        cpu->stages[DRF].stage_units[1].valid = 0;
                    cpu->stages[F].stalled = 1;
                    cpu->branch = Waiting;
                }
            }
            print_stage_content(DRF, j, unit, j == 0 ? "\t" : "\n", cpu->f);

            if (!stage->stalled) {
                append_unit(*unit, cpu->reserve_buffer);
                unit->valid = 0;
            }
        }
    }

    if (reservation_station_full_flag)
        cpu->full_reservation_count += 1;
    if (reorder_buffer_full_flag)
        cpu->full_reorder_count += 1;

    // Tip. 
    // Either one stage is not completed, 
    // the pipeline must be stalled at ID stage. 
    // Read the project description. 

    return 0;
}
