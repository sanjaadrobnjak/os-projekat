//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Semafor.hpp"
#include "../h/print.hpp"


void Riscv::popSppSpie()
{
    if(TCB::running->sisNit==0){
        mc_sstatus(SSTATUS_SPP);
    } else{
        ms_sstatus(SSTATUS_SPP);
    }
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
        __asm__ volatile("mv %0, a0" : "=r"(broj)); //a0 je kod sis poziva, br iz prve kolone tabele, upis vrednosti a0 u promenljivu broj

        if(broj==0x01){     //MEM_ALLOC
            //void* mem_alloc(size_t size);
            void* povratna;
            size_t size;
            __asm__ volatile("mv %0, a1":"=r"(size)); //u size mi je upisana vr a1, tj prvi parametar sis poziva
            povratna=MemoryAllocator::mem_alloc(size);
            __asm__ volatile("mv a0, %0"::"r"(povratna));   //u a0 povratnu vrednost upisujem promenljivu povratna
        } else if(broj==0x02){  //MEM_FREE
            //int mem_free(void* adresa);
            int povratna;
            void* adresa;
            __asm__ volatile("mv %0, a1":"=r"(adresa));
            povratna=MemoryAllocator::mem_free(adresa);
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x11){ //THREAD_CREATE
            //int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);
            int povratna;
            TCB** handle;
            void(*start_routine)(void*);
            void* argument;

            __asm__ volatile("mv %0, a1":"=r"(handle));
            __asm__ volatile("mv %0, a2":"=r"(start_routine));
            __asm__ volatile("mv %0, a3":"=r"(argument));
            /*__asm__ volatile("ld %0, 11*8(fp)" : "=r"(handle));

            __asm__ volatile("ld %0, 12*8(fp)" : "=r"(start_routine));

            __asm__ volatile("ld %0, 13*8(fp)" : "=r"(argument));*/

            *handle=TCB::createThread(start_routine, argument);
            if(*handle== nullptr) {
                povratna=-1;
            } else{
                povratna=0;
            }
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x12){  //THREAD_EXIT
            //int thread_exit();
            int povratna;
            povratna=TCB::exit();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x13){  //THREAD_DISPATCH()
            TCB::dispatch();
        } else if(broj==0x21){  //SEM_OPEN
            //int sem_open(semt* handle, unsigned init);
            int povratna;
            Semafor** handle;
            unsigned init;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            __asm__ volatile("mv %0, a2":"=r"(init));
            *handle=Semafor::semOpen(init);
            if(*handle!= nullptr) povratna=0;
            else povratna=-1;
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x22){  //SEM_CLOSE
            //int sem_close(sem_t handle);
            Semafor* handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            int povratna=handle->semClose();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x23){  //SEM_WAIT
            //int sem_wait(sem_t id);
            Semafor* handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            int povratna=handle->wait();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x24){  //SEM_SIGNAL
            //int sem_signal(sem_t id);
            Semafor* handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            int povratna=handle->signal();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x26){  //SEM_TRYWAIT
            //int sem_tryWait(sem_t id);
            Semafor* handle;
            __asm__ volatile("mv %0, a1":"=r"(handle));
            int povratna=handle->tryWait();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x41){    //GETC
            //char getc();
            char povratna;
            povratna=__getc();
            __asm__ volatile("mv a0, %0"::"r"(povratna));
        } else if(broj==0x42){  //PUTC
            //void putc(char);
            char slovo;
            __asm__ volatile("mv %0, a1":"=r"(slovo));
            __putc(slovo);
        }
        __asm__ volatile("sd a0, 10*8(s0)");
        w_sstatus(sstatus);
        w_sepc(sepc);
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000001UL)    //spoljasnji softverski prekid
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        //mc_sip(SIP_SSIP);
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
        mc_sip(SIP_SSIP);
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