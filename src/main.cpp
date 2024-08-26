//
// Created by os on 8/23/24.
//
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../test/printing.hpp"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"



extern void userMain();
//extern void producerConsumer_C_API();

int main(){
    //printStringg("Sanja\n");
    //printStringg("POCETAK PROGRAMA!\n");

    MemoryAllocator::init();
    TCB *threads[2];
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    threads[0] = TCB::createThread(nullptr, nullptr);
    threads[0]->sisNit=1;

    //printStringg("Main nit je kreirana\n");

    threads[1] = TCB::createThread((void (*)(void *))(&userMain), nullptr);
    //threads[1] = TCB::createThread((void (*)(void *))(&producerConsumer_C_API), nullptr);
    //printStringg("userMain nit je kreirana\n");
    TCB::running = threads[0];

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() ))
    {
        thread_dispatch();
    }



    //printStringg("KRAJ PROGRAMA!\n");*/


    return 0;
}