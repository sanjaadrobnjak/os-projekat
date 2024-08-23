//
// Created by os on 8/20/24.
//

#include "../h/syscall_c.h"


void* mem_alloc(size_t size){
    __asm__ volatile("mv a1, %0"::"r"(size));   //u a1 upisujem size
    __asm__ volatile("mv a0, %0"::"r"(0x01));   //u a0 upisujem 0x01
    __asm__ volatile("ecall");
    void* povratna;
    __asm__ volatile("mv %0, a0":"=r"(povratna));   //u povratnu vrednost upisujem a0

    return povratna;
}

int mem_free(void* adresa){
    __asm__ volatile("mv a1, %0"::"r"(adresa)); //u a1 upisujem adresa
    __asm__ volatile("mv a0, %0"::"r"(0x02));   //u a0 upisujem 0x02
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0, a0":"=r"(povratna));
    return povratna;
}



char getc() {
    __asm__ volatile("mv a0, %0"::"r"(0x41));
    __asm__ volatile("ecall");
    char povratna;
    __asm__ volatile("mv %0, a0":"=r"(povratna));
    return povratna;
}

void putc(char slovo){
    __asm__ volatile("mv a1, %0"::"r"(slovo));
    __asm__ volatile("mv a0, %0"::"r"(0x42));
    __asm__ volatile("ecall");
}