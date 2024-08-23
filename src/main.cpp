//
// Created by os on 8/23/24.
//

#include "../h/print.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"

int main(){
    printString("Sanja\n");
    printString("POCETAK PROGRAMA!\n");
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);    //postavljam sta mi  je prekidna rutina
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);  //maskiranjem omogucavam prekide



    int n = 10;
    char* niz = (char*)mem_alloc(n* sizeof(char));
    if(niz == nullptr)
        printString("Neuspesna alokacija! \n");
    else{
        printString("Alociran je prostor na adresi ");
        printInteger((uint64)niz);
        putc('\n');
    }

    for(int i=0;i<n;i++)
        niz[i] = 'd';

    for(int i = 0; i < n; i++){
        putc(niz[i]);
        putc(' ');
    }

    putc('\n');
    int status = mem_free(niz);
    if(status != 0)
        printString("Neuspesno brisanje! \n");
    else
        printString("Uspesno brisanje! \n");


    // excpected output == >
    // d d d d d d d d d d
    // Uspesno brisanje!

    printString("KRAJ PROGRAMA!\n");
    return 0;
}