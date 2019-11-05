//
// Created by wvv on 2019/10/27.
//

#include "hl_shell.h"
#include "hl_vm_core.h"
#include "hl_vm_debug.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <io.h>
typedef enum{
    SHELL_IDLE,SHELL_INPUT
}SHELL_STATE;
void hl_vm_input(struct HLVM *vm)
{
    int i=0;
    int op,opd;
    hl_vm_init(vm);
    puts("INPUT FORMAT:A BBB");
    puts("A is the operator, BBB is the operand, both in hex format, without 0x");
    puts("e to exit");
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
        printf("(%03d) 0x%02x%06x\n",i,op,opd); /*echo to user*/
        vm->_rom[i]=(op<<24)|opd; /*op(8bit) operand(24bit)*/
        i++;
    }
    puts("input finished.");
}
void hl_shell_menu()
{
    printf("\tHLVM v%04x\n",HLVM_VER);
    puts("type [command] to use.");
    puts("1. [vm] input code manual");
    puts("2. [rom] view rom");
    puts("3. [ram] view ram");
    puts("4. [run] run vm");
    puts("5. [ls] list .vm code");
    puts("6. [load xx] load xx.vm");
    puts("");

}
void hl_shell_main()
{

    struct HLVM vm;
    hl_vm_init(&vm);
    while (1)
    {
        hl_shell_menu();
        printf("input?");
        char buf[40],op[20],op2[20];
        gets(buf);/*for robust input, line input*/
        int ret=sscanf(buf,"%s %s",&op,&op2); /*if input valid, ret equal 2*/
        if(ret<1) /*input finished*/
        {
            break;
        }
        if(strcmp(op,"rom")==0)
        {
            hl_vm_rom_show(&vm);
        }
        else if(strcmp(op,"ram")==0)
        {
            hl_vm_dump(&vm);
        }
        else if(strcmp(op,"vm")==0)
        {
            hl_vm_input(&vm);
        }
        else if(strcmp(op,"run")==0)
        {
            hl_vm_start(&vm);
            while(hl_vm_step(&vm)==HLVM_OK)
            {
                system("cls");
                printf("step %6ld\n",vm.ticks);
                printf("->>%08x<<-\n",vm._rom[vm.pc]);
                hl_vm_dump(&vm);
                fflush(stdout);
                usleep(1000000); /*1ms*/
            }
            puts("\thalt.");
        }
        else if(strcmp(op,"ls")==0)
        {
            system("dir /b *.vm");
        }
        else if(strcmp(op,"load")==0)
        {
            int len=strlen(op2);
            op2[len]='.';
            op2[len+1]='v';
            op2[len+2]='m';
            op2[len+3]=0;
            FILE *pf=fopen(op2,"r");
            if(pf == NULL){
                printf("文件 %s 打开错误", op2);
                continue;
            }
            char buf[30];
            int i=0;
            hl_vm_init(&vm);
            while(!feof(pf))
            {
                char *p=fgets(buf,30,pf);
                if(p!=NULL)
                {
                    int a,b;
                    int ret=sscanf(buf,"%x %x",&a,&b); /*if input valid, ret equal 2*/
                    if(ret!=2) /*input finished*/
                    {
                        break;
                    }
                    printf("(%03d) < 0x%02x%06x\n",i,a,b); /*echo to user*/
                    vm._rom[i]=(a<<24)|b; /*op(8bit) operand(24bit)*/
                    i++;
                }
            }
            puts("done.");
        }
    }
    puts("exit.");
}