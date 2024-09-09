//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Semafor.hpp"


void Riscv::popSppSpie()
{
    changeStatus(TCB::running->sisNit);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)   //ecall iz korisnickog ili sistemskog rezima
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 broj;
        __asm__ volatile("mv %0, a0" : "=r"(broj));

        void* povVr;
        int povratna;
        Semafor* handleS;

        switch (broj) {
            case 0x01:  //mem_alloc
                size_t size;
                __asm__ volatile("mv %0, a1":"=r"(size));
                povVr=MemoryAllocator::mem_alloc(size);
                __asm__ volatile("mv a0, %0"::"r"(povVr));
                break;
            case 0x02:  //mem_free
                void* adresa;
                __asm__ volatile("mv %0, a1":"=r"(adresa));
                povratna=MemoryAllocator::mem_free(adresa);
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x11:  //thread_create
                TCB** handle;
                void(*start_routine)(void*);
                void* argument;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(handle));
                __asm__ volatile("ld %0, 12*8(fp)" : "=r"(start_routine));
                __asm__ volatile("ld %0, 13*8(fp)" : "=r"(argument));

                *handle=TCB::createThread(start_routine, argument, 2);
                if(*handle== nullptr) {
                    povratna=-1;
                } else{
                    povratna=0;
                }
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x12:    //thread_exit
                povratna=TCB::exit();
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x13:  //thread_dispatch
                TCB::timeSliceCounter=0;
                TCB::dispatch();
                break;
            case 0x21:  //sem_open
                Semafor** handleSem;
                unsigned init;
                __asm__ volatile("mv %0, a1":"=r"(handleSem));
                __asm__ volatile("mv %0, a2":"=r"(init));
                *handleSem=Semafor::semOpen(init);
                if(*handleSem!= nullptr) povratna=0;
                else povratna=-1;
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x22:  //sem_close

                __asm__ volatile("mv %0, a1":"=r"(handleS));
                povratna=handleS->semClose();
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x23:  //sem_wait
                __asm__ volatile("mv %0, a1":"=r"(handleS));
                if(handleS== nullptr) povratna=-1;
                else povratna=handleS->wait();
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x24:  //sem_signal
                __asm__ volatile("mv %0, a1":"=r"(handleS));
                if(handleS== nullptr) povratna=-1;
                else povratna=handleS->signal();
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x26:  //sem_tryWait
                __asm__ volatile("mv %0, a1":"=r"(handleS));
                if(handleS== nullptr) povratna=-1;
                else povratna=handleS->tryWait();
                __asm__ volatile("mv a0, %0"::"r"(povratna));
                break;
            case 0x41:  //getc
                char pov;
                pov=__getc();
                __asm__ volatile("mv a0, %0"::"r"(pov));
                break;
            case 0x42: //putc
                char slovo;
                __asm__ volatile("mv %0, a1":"=r"(slovo));
                __putc(slovo);
                break;

        }
        __asm__ volatile("sd a0, 10*8(s0)");
        w_sstatus(sstatus);
        w_sepc(sepc);
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000001UL)    //spoljasnji softverski prekid
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)    //spoljasnji hardverski prekid
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }
}