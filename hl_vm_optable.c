//
// Created by winxo on 2019/11/5.
//

#include "hl_vm_optable.h"
static inline HLVM_RET _ldr(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=MEM_SIZE || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->reg=vm->_mems[operand];
    return HLVM_OK;
}
static inline HLVM_RET _str(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=MEM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->_mems[operand]=vm->reg;
    return HLVM_OK;
}
static inline HLVM_RET _sreg(struct HLVM *vm)
{
    vm->reg=vm->_rom[vm->pc].operand;
    return HLVM_OK;
}
static inline HLVM_RET _b(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=ROM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->pc=operand-1;
    return HLVM_OK;
}
static inline HLVM_RET _bl(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=ROM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    vm->lr=vm->pc+1;
    vm->pc=operand-1;
    return HLVM_OK;
}
static inline HLVM_RET _bn(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=ROM_SIZE  || operand<0)
    {
        return HLVM_PC_OUT_RANGE;
    }
    if(vm->reg<0) {
        vm->pc = operand - 1;
    }
    return HLVM_OK;
}
static inline HLVM_RET _bz(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    if(operand>=ROM_SIZE  || operand<0)
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
    s32 operand=vm->_rom[vm->pc].operand;
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
    if(vm->sp<MEM_SIZE-STACK_SIZE)
    {
        return HLVM_SP_OUT_RANGE;
    }
    vm->_mems[vm->sp--]=vm->reg;
    return HLVM_OK;
}
static inline HLVM_RET _pop(struct HLVM *vm)
{
    if(vm->sp>=MEM_SIZE-1)
    {
        return HLVM_SP_OUT_RANGE;
    }
    vm->reg=vm->_mems[++vm->sp];
    return HLVM_OK;
}
static inline HLVM_RET _slp(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
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
    }
    return HLVM_OK;
}
static inline HLVM_RET _add(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg+=operand;
    return HLVM_OK;
}
static inline HLVM_RET _sub(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg-=operand;
    return HLVM_OK;
}
static inline HLVM_RET _mul(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg*=operand;
    return HLVM_OK;
}
static inline HLVM_RET _div(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg/=operand;
    return HLVM_OK;
}
static inline HLVM_RET _mod(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg%=operand;
    return HLVM_OK;
}
static inline HLVM_RET _inc(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg++;
    return HLVM_OK;
}
static inline HLVM_RET _dec(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg--;
    return HLVM_OK;
}
static inline HLVM_RET _neg(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg=-vm->reg;
    return HLVM_OK;
}
static inline HLVM_RET _and(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg=vm->reg&operand;
    return HLVM_OK;
}
static inline HLVM_RET _or(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg=vm->reg|operand;
    return HLVM_OK;
}
static inline HLVM_RET _xor(struct HLVM *vm)
{
    s32 operand=vm->_rom[vm->pc].operand;
    vm->reg=vm->reg^operand;
    return HLVM_OK;
}
HLVMExec _op_tables[]={
        NULL,_ldr,_str,_sreg,_add,_sub,_mul,_div,_mod,_inc,_dec,_neg,_and,_or,_xor,
        _b,_bl,_bz,_bn,_int,_halt,_push,_pop,_slp
};