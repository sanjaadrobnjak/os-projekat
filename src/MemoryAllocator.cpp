//
// Created by os on 8/7/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::SlobodniBlokovi* MemoryAllocator::pocetak;

void *MemoryAllocator::mem_alloc(size_t size) {
    if(size<=0) return nullptr;
    if(pocetak== nullptr) return nullptr;

    int blokovi=size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE!=0){
        blokovi++;
    }


    size_t prostor=blokovi*MEM_BLOCK_SIZE;

    SlobodniBlokovi* trenutni=pocetak;
    SlobodniBlokovi* prethTren= nullptr;
    SlobodniBlokovi* bestFit= nullptr;
    SlobodniBlokovi* prethBF= nullptr;
    while(trenutni!= nullptr){
        if(trenutni->velicina==prostor){
            bestFit=trenutni;
            prethBF=prethTren;
            break;
        } else{
            if(trenutni->velicina>prostor){
                if(bestFit== nullptr || bestFit->velicina>trenutni->velicina) {
                    bestFit = trenutni;
                    prethBF = prethTren;
                }
            }
            prethTren=trenutni;
            trenutni=trenutni->sledeci;
        }
    }

    if(bestFit== nullptr) return nullptr;


    //imam sig slobodan prostor za alok.
    size_t ostatak=bestFit->velicina-prostor;
    if(ostatak>=MEM_BLOCK_SIZE+sizeof (SlobodniBlokovi)){

        bestFit->velicina=prostor;

        size_t adr=sizeof (SlobodniBlokovi)+prostor;
        auto* noviBlok=(SlobodniBlokovi*)((char *)bestFit+adr);
        if(prethBF) prethBF->sledeci=noviBlok;
        else pocetak=noviBlok;
        noviBlok->sledeci=bestFit->sledeci;
        noviBlok->velicina=ostatak-sizeof (SlobodniBlokovi);
    } else{
        if(prethBF) prethBF->sledeci=bestFit->sledeci;
        else pocetak=bestFit->sledeci;
    }
    bestFit->sledeci= nullptr;

    return (char *)bestFit+sizeof (SlobodniBlokovi);
}

void MemoryAllocator::init() {
    pocetak=(SlobodniBlokovi*)HEAP_START_ADDR;
    pocetak->sledeci= nullptr;
    pocetak->velicina=(size_t)((char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR);
}

int MemoryAllocator::mem_free(void *adresa) {

    if(adresa>=HEAP_END_ADDR || adresa<HEAP_START_ADDR) return -1;

    SlobodniBlokovi* trenutni;
    if(pocetak== nullptr){
        trenutni= nullptr;
    } else if((char*)pocetak>adresa){
        trenutni= nullptr;
    } else{
        for(trenutni=pocetak; trenutni->sledeci!= nullptr && adresa>(char*)(trenutni->sledeci); trenutni=trenutni->sledeci);
    }

    auto* noviBlok=(SlobodniBlokovi*)adresa;

    if(trenutni!= nullptr){
        noviBlok->sledeci=trenutni->sledeci;
        trenutni->sledeci=noviBlok;
    }
    if(spoji(noviBlok));
    if(spoji(trenutni));
    return 0;
}

bool MemoryAllocator::spoji(MemoryAllocator::SlobodniBlokovi *tren) {
    if(tren== nullptr) return 0;

    if (tren->sledeci && (char*)tren+tren->velicina==(char*)(tren->sledeci)){
        tren->velicina=tren->velicina+tren->sledeci->velicina;
        tren->sledeci=tren->sledeci->sledeci;
        return true;
    }
    return false;
}

