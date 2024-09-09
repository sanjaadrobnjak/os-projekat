//
// Created by os on 8/21/24.
//

#ifndef PROJECT_BASE_V1_1_SANJA_SEMAFOR_HPP
#define PROJECT_BASE_V1_1_SANJA_SEMAFOR_HPP

#include "../h/list.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.h"


class Semafor {
protected:
    struct blokiraneNiti{
        blokiraneNiti* sledeci;
        TCB* nit;
    };
private:
    int val;
    friend class Riscv;
    bool zatvoren;
    blokiraneNiti* pocetak;
public:
    explicit Semafor(int init=1): val(init), zatvoren(false), pocetak(nullptr) {}

    static Semafor* semOpen(unsigned init);
    int semClose();

    int wait();
    int signal();
    int tryWait();
};


#endif //PROJECT_BASE_V1_1_SANJA_SEMAFOR_HPP
