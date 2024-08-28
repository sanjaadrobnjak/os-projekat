//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"



// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void *);    //Body je pokazivac na funkciju ciji je argument tipa void* i koja nema povratnu vrednost

    static TCB *createThread(Body body, void* argument);

    static void yield();
    static void dispatch();
    static int exit();

    static TCB *running;

    int sisNit;
    bool blokirana;

private:
    TCB(Body body, uint64 timeSlice, void* arg);

    struct Context
    {
        uint64 ra;  //dokle je nit stigla kako bi mogla da se vrati kasnije
        uint64 sp;
    };

    Body body;
    void* argument;
    uint64 *stack;
    Context context;
    uint64 timeSlice;   //broj perioda koji dobija nit svaki put kad joj je dodeljen procesor
    bool finished;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);



    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
