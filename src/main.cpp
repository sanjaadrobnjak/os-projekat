//
// Created by os on 8/23/24.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/print.hpp"


extern void userMain();

void main(){

    MemoryAllocator::init();

    TCB* nit;
    TCB* testNit;

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    if(thread_create(&nit, nullptr, nullptr)==-1){
        printStringg("SANJA:Greska pri kreiranju niti nit!!!\n");
    }
    TCB::running=nit;

    if(thread_create(&testNit, (void (*)(void *))(&userMain), nullptr)==-1){
        printStringg("SANJA:Greska pri kreiranju niti testNit!!!\n");
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (1)
    {
        if(testNit->isFinished()) break;
        thread_dispatch();
    }

    printStringg("SANJA ZAVRSILA IZVRSAVANJE PROGRAMA!\n");
}