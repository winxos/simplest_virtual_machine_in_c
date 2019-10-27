//
// Created by winxo on 2019-10-27.
//

#include <stdio.h>
#include "hl_vm_test.h"
#include "hl_vm_core.h"
void hl_vm_test() {
    if(hl_vm__core_test()==FALSE)
    {
        puts("VM CORE TEST FAILED.");
    }
    else
    {
        puts("VM CORE TEST PASSED.");
    }
}
