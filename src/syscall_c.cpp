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

int thread_create( thread_t* handle, void(*start_routine)(void*), void* arg){

    __asm__ volatile("mv a3, %0"::"r"(arg));
    __asm__ volatile("mv a2, %0"::"r"(start_routine));
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("mv a0, %0"::"r"(0x11));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int thread_exit(){
    __asm__ volatile("mv a0, %0"::"r"(0x12));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0, a0":"=r"(povratna));
    return povratna;
}

void thread_dispatch(){
    __asm__ volatile("mv a0, %0"::"r"(0x13));
    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init){
    __asm__ volatile("mv a2, %0"::"r"(init));
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("mv a0, %0"::"r"(0x21));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int sem_close(sem_t handle){
    __asm__ volatile("mv a1, %0"::"r"(handle));
    __asm__ volatile("mv a0, %0"::"r"(0x22));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int sem_wait(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("mv a0, %0"::"r"(0x23));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int sem_signal(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("mv a0, %0"::"r"(0x24));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int sem_timedwait(sem_t id, time_t timeout){
    __asm__ volatile("mv a2, %0"::"r"(timeout));
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("mv a0, %0"::"r"(0x25));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int sem_tryWait(sem_t id){
    __asm__ volatile("mv a1, %0"::"r"(id));
    __asm__ volatile("mv a0, %0"::"r"(0x26));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
    return povratna;
}

int time_sleep(time_t t){
    __asm__ volatile("mv a1, %0"::"r"(t));
    __asm__ volatile("mv a0, %0"::"r"(0x31));
    __asm__ volatile("ecall");
    int povratna;
    __asm__ volatile("mv %0,a0":"=r"(povratna));
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