//
// Created by os on 8/21/24.
//

#include "../h/Semafor.hpp"



Semafor* Semafor::semOpen(unsigned int init) {
    return new Semafor((int)init);
}

int Semafor::semClose() {
    while(pocetak!= nullptr){
        pocetak->nit->blokirana=false;
        blokiraneNiti* temp = pocetak;
        pocetak = pocetak->sledeci;
        delete temp;
    }
    zatvoren= true;
    return 0;
}

int Semafor::wait() {
    if(zatvoren) return -1;
    val--;
    if(val<0){
        TCB::running->blokirana= true;
        auto* novaNit=new blokiraneNiti{nullptr, TCB::running};

        if(pocetak== nullptr){
            pocetak=novaNit;
        } else{
            blokiraneNiti* posl=pocetak;
            while(posl->sledeci!= nullptr){
                posl=posl->sledeci;
            }
            posl->sledeci=novaNit;
        }
        thread_dispatch();

    }
    return 0;
}

int Semafor::signal() {
    if(zatvoren) return -1;
    val++;
    if(val<=0){
        if(pocetak != nullptr){
            TCB* spremnaNit=pocetak->nit;
            spremnaNit->blokirana= false;
            pocetak=pocetak->sledeci;
            Scheduler::put(spremnaNit);
        }
    }
    return 0;
}

int Semafor::tryWait() {
    if(zatvoren) return -1;
    if(val>=1){
        val--;
        return 0;
    }
    return 1;
}
