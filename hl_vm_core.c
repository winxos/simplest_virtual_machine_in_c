//
// Created by zx on 2019/10/26.
//

#include "hl_vm_core.h"
#include <string.h>

struct HLVMAction{
    u8 code;
    HLVMExec exec;
};
static inline HLVM_RET _load(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->reg=vm->_mems[operand].operand;
    return HLVM_OK;
}
static inline HLVM_RET _store(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->_mems[operand].operand=vm->reg;
    return HLVM_OK;
}
static inline HLVM_RET _set_reg(struct HLVM *vm)
{
    vm->reg=vm->_mems[vm->pc].operand;
    return HLVM_OK;
}
static inline HLVM_RET _jmp(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->pc=operand-1;
    return HLVM_OK;
}
static inline HLVM_RET _jmpn(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    if(vm->reg<0) {
        vm->pc = operand - 1;
    }
    return HLVM_OK;
}
static inline HLVM_RET _jmpz(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    if(vm->reg==0) {
        vm->pc = operand - 1;
    }
    return HLVM_OK;
}
static inline HLVM_RET _int(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand>=INTERRUPT_SIZE  || operand<0)
    {
        return HLVM_PARA_OUT_RANGE;
    }
    HLVMExec p=vm->_interrupts[operand];
    if(p!=NULL)
    {
        p(vm);
    }
    return HLVM_OK;
}
static inline HLVM_RET _halt(struct HLVM *vm)
{
    return HLVM_HALT;
}
static inline HLVM_RET _push(struct HLVM *vm)
{
    if(vm->sp>=STACK_SIZE-1)
    {
        return HLVM_SP_OUT_RANGE;
    }
    vm->_stack[vm->sp++]=vm->reg;
    return HLVM_OK;
}
static inline HLVM_RET _pop(struct HLVM *vm)
{
    if(vm->sp<=0)
    {
        return HLVM_SP_OUT_RANGE;
    }
    vm->reg=vm->_stack[--vm->sp];
    return HLVM_OK;
}
static inline HLVM_RET _sleep(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    if(operand<0)
    {
        return HLVM_PARA_OUT_RANGE;
    }
    if(operand==0)
    {
        return HLVM_OK;
    }
    if(vm->state==HLVM_STATE_RUNNING)
    {
        vm->_sleeping=operand;
        vm->state=HLVM_STATE_SLEEPING;
    } else if(vm->state==HLVM_STATE_SLEEPING)
    {
        vm->_sleeping--;
        if(vm->_sleeping<=0)
        {
            vm->state=HLVM_STATE_RUNNING;
        }
    }
    return HLVM_OK;
}
static inline HLVM_RET _add(struct HLVM *vm)
{
    s32 operand=vm->_mems[vm->pc].operand;
    vm->reg+=operand;
    return HLVM_OK;
}
static struct HLVMAction _tables[]={
        {0x20,_load},
        {0x21,_store},
        {0x22,_set_reg},
        {0x30,_jmp},
        {0x31,_jmpn},
        {0x32,_jmpz},
        {0x33,_int},
        {0x34,_halt},
        {0x40,_push},
        {0x41,_pop},
        {0x42,_sleep},
        {0x50,_add},
        {}
};
static HLVMExec _find_exec(u8 code)
{
    struct HLVMAction *p=_tables;
    while(p!=NULL)
    {
        if(p->code==code)
        {
            return p->exec;
        }
        p++;
    }
}
void hl_vm_init(struct HLVM *vm) {
    memset(vm,0, sizeof(struct HLVM));
}

HLVM_RET hl_vm_step(struct HLVM *vm) {
    vm->ticks++;
    HLVMExec exec= _find_exec(vm->_mems[vm->pc].operator);
    if(exec==NULL)
    {
        return HLVM_INSTRUCTION_NOT_EXIST;
    }
    HLVM_RET ret=exec(vm);
    if(ret==HLVM_STATE_RUNNING)
    {
        vm->pc++;
    }
    if(vm->pc>=MEM_SIZE)
    {
        return HLVM_PC_OUT_RANGE;
    }
    return ret;
}

bool hl_vm__core_test() {
    struct HLVM vm={._mems={{0x20,1},{0x21,4},{0x22,3}}};
    hl_vm_step(&vm);
    hl_vm_step(&vm);
    hl_vm_step(&vm);
}
