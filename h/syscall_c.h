//
// Created by os on 8/20/24.
//

#ifndef PROJECT_BASE_V1_1_SANJA_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SANJA_SYSCALL_C_H

#include "../lib/hw.h"


void* mem_alloc(size_t size);

int mem_free(void* );



const int EOF=-1;
char getc();

void putc(char );


#endif //PROJECT_BASE_V1_1_SANJA_SYSCALL_C_H
