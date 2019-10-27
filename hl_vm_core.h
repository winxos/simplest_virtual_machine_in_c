//
// Created by zx on 2019/10/26.
//

#ifndef HLVM_HL_VM_CORE_H
#define HLVM_HL_VM_CORE_H

#include "typedef.h"
typedef u8 HLVM_Ret;
enum HLVMOpSet{
    LOAD_ = 0x20, STORE_, SREG_,
    ADD_ = 0x30, SUB_, MUL_, DIV_, MOD_, INC_, DEC_, NEG_,
    JMP_ = 0x40, JMPN_, JMPZ_,INT_, HALT_,
    AND_ = 0x50, OR_, XOR_,
    PUSH_ = 0x60, POP_,
    PMOD = 0x70, DWP, DRP, AWP, ARP,
    SLP = 0x80,
};
struct HLVMCode{
    u8 operator;
    u32 operand;
};
#define MEM_SIZE  100
#define STACK_SIZE 20
struct HLVM{
    u32 reg;
    struct HLVMCode _mems[MEM_SIZE];
    u32 _stack[STACK_SIZE];
};
HLVM_Ret hl_vm_step(struct HLVM *vm)
{

}
#endif //HLVM_HL_VM_CORE_H
