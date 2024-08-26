//
// Created by os on 8/7/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::SlobodniBlokovi* MemoryAllocator::pocetak;

void *MemoryAllocator::mem_alloc(size_t size) {
    if(size<=0) return nullptr; //neispravan unos prostora za alokaciju
    if(pocetak== nullptr) return nullptr; //nema slobodnog prostora za alokaciju

    //prostor koji zeli da se alocira u broju blokova
    int blokovi=size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE!=0){
        blokovi++;
    }

    //prostor koji zeli da se alocira u bajtovima poravnat na min br blokova
    size_t prostor=blokovi*MEM_BLOCK_SIZE;

    //prolazim kroz listu slobodnih blokova da vidim ima li mesta za novu alokaciju
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

    if(bestFit== nullptr) return nullptr;   //nema slobodnog prostora za novu alokaciju


    //imam sig slobodan prostor za alok.
    size_t ostatak=bestFit->velicina-prostor;
    if(ostatak>=MEM_BLOCK_SIZE+sizeof (SlobodniBlokovi)){
        //imam dovoljno prostora da ostatak ulancam u listu slobodnih
        bestFit->velicina=prostor;

        size_t adr=sizeof (SlobodniBlokovi)+prostor;   //poziciija gde pocinje novi blok nakon alociranog dela
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

    //!!!ARGUMENT MORA BITI VRACENA VR IZ mem_alloc!!!

    SlobodniBlokovi* trenutni;
    if(pocetak== nullptr){  //nema vise slobodnog mesta uopste
        trenutni= nullptr;
    } else if((char*)pocetak>adresa){   //sve do pocetka je zauzeto
        trenutni= nullptr;
    } else{
        //trazim mesto u listi slobodnih da se po adresi smesti addr, tj da ide nakon trenutni
        for(trenutni=pocetak; trenutni->sledeci!= nullptr && adresa>(char*)(trenutni->sledeci); trenutni=trenutni->sledeci);
    }

    //ubacujem blok nakon bloka trenutni
    auto* noviBlok=(SlobodniBlokovi*)adresa;
    //noviBlok->velicina=.. !!!PROVERITI!!!
    if(trenutni!= nullptr){
        noviBlok->sledeci=trenutni->sledeci;
        trenutni->sledeci=noviBlok;
    }
    spoji(noviBlok);
    spoji(trenutni);
    return 0;
}

int MemoryAllocator::spoji(MemoryAllocator::SlobodniBlokovi *tren) {
    if(tren== nullptr) return 0;

    //proveravam da li nakon tren slob bloka pocinje jos jedan slob blok kako bih prosirila tren slobodni fragment
    if (tren->sledeci && (char*)tren+tren->velicina==(char*)(tren->sledeci)){
        tren->velicina=tren->velicina+tren->sledeci->velicina;
        tren->sledeci=tren->sledeci->sledeci;
        return 1;
    }
    return 0;
}

