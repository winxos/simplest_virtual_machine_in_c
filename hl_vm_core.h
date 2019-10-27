//
// Created by zx on 2019/10/26.
//

#ifndef HLVM_HL_VM_CORE_H
#define HLVM_HL_VM_CORE_H

#include "typedef.h"

enum HLVMInt{
    HL_INT_UART_TX,HL_INT_DWIO,HL_INT_DCMOTOR,HL_INT_STEPPER
};
enum HLVMOpSet{
    LOAD_ = 0x20, STORE_, SREG_,
    ADD_ = 0x30, SUB_, MUL_, DIV_, MOD_, INC_, DEC_, NEG_,
    JMP_ = 0x40, JMPN_, JMPZ_,INT_, HALT_,
    AND_ = 0x50, OR_, XOR_,
    PUSH_ = 0x60, POP_,
    SLP_ = 0x80,
};
struct HLVMCode{
    u8 operator;
    s32 operand;
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
#define STACK_SIZE 20
#define INTERRUPT_SIZE 10
struct HLVM{
    struct HLVMCode _mems[MEM_SIZE];
    u32 _stack[STACK_SIZE];
    void* _interrupts[INTERRUPT_SIZE]; /*exec pointer*/
    s32 reg;
    u32 ticks;
    u32 pc;
    u32 sp;
    HLVM_STATE state;
    u32 _sleeping;
};
typedef HLVM_RET (*HLVMExec)(struct HLVM* vm);

void hl_vm_init(struct HLVM *vm);
HLVM_RET hl_vm_step(struct HLVM *vm);
bool hl_vm__core_test();
#endif //HLVM_HL_VM_CORE_H
