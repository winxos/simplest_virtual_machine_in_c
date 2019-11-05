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
    struct HLVMCode _rom[ROM_SIZE];/*for code*/
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
typedef HLVM_RET (*HLVMExec)(struct HLVM* vm);

void hl_vm_init(struct HLVM *vm);
HLVM_RET hl_vm_step(struct HLVM *vm);
bool hl_vm__core_test();
#endif //HLVM_HL_VM_CORE_H
