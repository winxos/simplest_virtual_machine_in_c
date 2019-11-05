//
// Created by zx on 2019/10/26.
//

#ifndef HLVM_HL_VM_CORE_H
#define HLVM_HL_VM_CORE_H

#include "typedef.h"
#define HLVM_VER 0x0103
enum HLVMInt{
    HL_INT_UART_TX,HL_INT_DWIO,HL_INT_DCMOTOR,HL_INT_STEPPER
};
typedef enum
{
    HLVM_STATE_IDLE,HLVM_STATE_RUNNING,HLVM_STATE_HALT,HLVM_STATE_SLEEPING,HLVM_STATE_ERROR
}HLVM_STATE;
typedef enum
{
    HLVM_OK,HLVM_INSTRUCTION_NOT_EXIST, HLVM_PC_OUT_RANGE,HLVM_SP_OUT_RANGE,HLVM_PARA_OUT_RANGE,HLVM_DIV_BY_ZERO,HLVM_HALT
}HLVM_RET;


#define MEM_SIZE  100
#define ROM_SIZE  100
#define INTERRUPT_SIZE 10
#define STACK_SIZE 20
struct HLVM{
    u32 _mems[MEM_SIZE];/*memories*/
    void* _interrupts[INTERRUPT_SIZE]; /*soft interrupt*/
    s32 reg;
    u32 pc;/*program counter*/
    u32 sp;/*stack pointer*/
    u32 lr;/*link reg*/
    u32 ticks;/*cpu time*/
    u32 _sleeping;
    HLVM_STATE state;
};
#define GET_OP(n) ((n&0xff000000)>>24)
#define GET_OPERAND(n) (n&0x00ffffff)
typedef HLVM_RET (*HLVMExec)(struct HLVM* vm);

void hl_vm_start(struct HLVM *vm);
void hl_vm_init(struct HLVM *vm);
HLVM_RET hl_vm_step(struct HLVM *vm);
bool hl_vm__core_test();
#endif //HLVM_HL_VM_CORE_H
