cmake_minimum_required(VERSION 3.15)
project(HLVM C)

set(CMAKE_C_STANDARD 99)

add_executable(HLVM main.c typedef.h hl_vm_core.c hl_vm_core.h hl_vm_manager.c hl_vm_manager.h)