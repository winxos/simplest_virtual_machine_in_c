//
// Created by winxo on 2019/11/5.
//

#include "hl_vm_debug.h"
#include <stdio.h>

#define COLS 10
#define TITLE_FORMAT "%10X"
#define DATA_FORMAT "  %08x"
#define COL_HEADER_FORMAT "\n%02d:"
#define REG_FMT "REG:%08d\tPC:%02d\tSP:%02d\tLR:%02d\tTICKS:%ld\n"
void hl_vm_dump(struct HLVM *vm) {
    int i;
    puts("RAM:");
    printf(REG_FMT,vm->reg,vm->pc,vm->sp,vm->lr,vm->ticks);
    for (i = 0; i < COLS; i++)
    {
        printf(TITLE_FORMAT, i);
    }
    for (i = 0; i < MEM_SIZE; i++)
    {
        if (i % COLS == 0)
        {
            printf(COL_HEADER_FORMAT, i / COLS*10);
        }
        printf(DATA_FORMAT, vm->_mems[i]);
    }
    puts("\n");
}
void hl_vm_rom_show(struct HLVM *vm) {
    int i;
    puts("ROM:");
    for (i = 0; i < COLS; i++)
    {
        printf(TITLE_FORMAT, i);
    }
    for (i = 0; i < MEM_SIZE; i++)
    {
        if (i % COLS == 0)
        {
            printf(COL_HEADER_FORMAT, i / COLS*10);
        }
        printf(DATA_FORMAT,vm->_rom[i]);
    }
    puts("\n");
}