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
    u32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE)
    {
        return HLVM_MEM_OVERFLOW;
    }
    vm->reg=vm->_mems[operand].operand;
}
static inline HLVM_RET _store(struct HLVM *vm)
{
    u32 operand=vm->_mems[vm->pc].operand;
    if(operand>=MEM_SIZE)
    {
        return HLVM_MEM_OVERFLOW;
    }
    vm->_mems[operand].operand=vm->reg;
}
static inline HLVM_RET _set_reg(struct HLVM *vm)
{
    vm->reg=vm->_mems[vm->pc].operand;
}

static struct HLVMAction _tables[]={
        {0x20,_load},
        {0x21,_store},
        {0x22,_set_reg},
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
    vm->pc++;
    if(vm->pc>=MEM_SIZE)
    {
        return HLVM_MEM_OVERFLOW;
    }
    return ret;
}

bool hl_vm__core_test() {
    struct HLVM vm={._mems={{0x20,1},{0x21,4},{0x22,3}}};
    hl_vm_step(&vm);
    hl_vm_step(&vm);
    hl_vm_step(&vm);
}
