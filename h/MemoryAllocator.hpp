//
// Created by os on 8/7/24.
//

#ifndef PROJECT_BASE_V1_1_SANJA_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_SANJA_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

//alokacija prostora implementirana pomocu best fit algoritma kontinualne alokacije
//ideja za alokaciju memorije uzeta sa 1og zadatka sa kolokvijuma u avgustu 2021. i 2og zadatka sa kolokvijuma oktobra 2020.
//ideja za dealokaciju memorije uzeta sa 2og zadatka na drugom kolokvijumu u septembu 2015.

class MemoryAllocator{

protected:
    //slobodni blokovi su uredjeni u jednostruko ulancanu listu (neuredjenu po vel, a uredjenu po adresi)
    //svaki elem liste je 1 slobodan deo memkoji na pocetku ima zaglavlje tipa SlobodniBlokovi
    //u kome je pok na sl takav zapis, kao i velicina slobodnog bloka iza ovog zaglavlja
    struct SlobodniBlokovi{
        SlobodniBlokovi* sledeci;
        size_t velicina; //u bajtovima, broj slobodnih blokova*MEM_BLK_SIZE
    };

private:
     static SlobodniBlokovi* pocetak;   //svi objekti klase MemoryAllocator dele isti pocetak

public:
    static void* mem_alloc(size_t size);
    static int mem_free(void*);
    static int spoji(SlobodniBlokovi* tren);

    static void init();


};

#endif //PROJECT_BASE_V1_1_SANJA_MEMORYALLOCATOR_HPP
