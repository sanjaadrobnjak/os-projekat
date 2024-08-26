//
// Created by os on 8/21/24.
//

#include "../h/Semafor.hpp"



Semafor* Semafor::semOpen(unsigned int init) {
    return new Semafor((int)init);
}

int Semafor::semClose() {
    while(pocetak!= nullptr){
        //lista blokiranih niti nije prazna, deblokiraj sve niti
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
        //suspendujem pozivajucu nit i stavljam je u listu blokiranih niti
        TCB::running->blokirana= true;
        auto* novaNit=new blokiraneNiti{nullptr, TCB::running};

        if(pocetak== nullptr){
            pocetak=novaNit;
        } else{
            blokiraneNiti* posl=pocetak;    //poslednji u listi blokiranih
            while(posl->sledeci!= nullptr){
                posl=posl->sledeci;
            }
            posl->sledeci=novaNit;
        }
        //uzimam novu nit iz reda spremnih i vrsim promenu konteksta
        thread_dispatch();

    }
    return 0;
}

int Semafor::signal() {
    if(zatvoren) return -1;
    val++;
    if(val<=0){
        //uzimam jednu nit iz liste blokiranih
        if(pocetak != nullptr){
            TCB* spremnaNit=pocetak->nit;
            spremnaNit->blokirana= false;
            pocetak=pocetak->sledeci;
            //ubacujem tu nit u red spremnih
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
