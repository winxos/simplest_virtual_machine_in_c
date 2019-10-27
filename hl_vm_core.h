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
    u32 operand;
};
#define MEM_SIZE  100
#define STACK_SIZE 20
struct HLVM{
    u32 reg;
    u32 ticks;
    u32 pc;
    u32 sp;
    struct HLVMCode _mems[MEM_SIZE];
    u32 _stack[STACK_SIZE];
};

typedef enum
{
    HLVM_OK,HLVM_INSTRUCTION_NOT_EXIST, HLVM_MEM_OVERFLOW,HLVM_STACK_OVERFLOW,HLVM_INT_NOT_EXIST,HLVM_DIV_BY_ZERO,HLVM_HALT
}HLVM_RET;

typedef HLVM_RET (*HLVMExec)(struct HLVM* vm);

void hl_vm_init(struct HLVM *vm);
HLVM_RET hl_vm_step(struct HLVM *vm);
bool hl_vm__core_test();
#endif //HLVM_HL_VM_CORE_H
