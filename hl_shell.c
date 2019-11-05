//
// Created by wvv on 2019/10/27.
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
    hl_vm_init(&vm);
    while (1)
    {
        printf("%03d?",i);/*input hint*/
        char buf[40];
        gets(buf);/*for robust input, line input*/
        int ret=sscanf(buf,"%x %x",&op,&opd); /*if input valid, ret equal 2*/
        if(ret!=2) /*input finished*/
        {
            break;
        }
        printf("(%03d) 0x%02x %08x\n",i,op,opd); /*echo to user*/
        vm._rom[i]=(op<<24)|opd; /*op(8bit) operand(24bit)*/
        i++;
    }
    puts("input finished.");
    hl_vm_rom_show(&vm);
    hl_vm_dump(&vm);
}