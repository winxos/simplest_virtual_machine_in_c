//
// Created by zx on 2019/10/27.
//

#include "hl_shell.h"
#include "hl_vm_core.h"
#include "hl_vm_debug.h"
#include <stdio.h>
#include <string.h>
typedef enum{
    SHELL_IDLE,SHELL_INPUT
}SHELL_STATE;

void hl_shell_main()
{
    int i=0;
    int op,opd;
    struct HLVM vm;
    memset(&vm,0, sizeof(vm));
    while (1)
    {
        printf("%03d?",i);
        char buf[40];
        gets(buf);
        int ret=sscanf(buf,"%x %x",&op,&opd);
        if(ret!=2)
        {
            break;
        }
        printf("(%03d) 0x%02x %08x\n",i,op,opd);
        vm._rom[i].operand=op;
        vm._rom[i].operator=opd;
        i++;
    }
    puts("input finished.");
    hl_vm_rom_show(&vm);
    hl_vm_dump(&vm);
}