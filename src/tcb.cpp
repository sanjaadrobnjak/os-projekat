//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* argument)
{
    return new TCB(body, TIME_SLICE, argument);
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->blokirana) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);


}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->argument);
    running->setFinished(true);
    TCB::yield();
}

int TCB::exit()
{
    running->setFinished(true);
    TCB::yield();
    return 0;
}

TCB::TCB(TCB::Body bodyy, uint64 timeSlicee, void *arg)
{
    this->body=bodyy;
    this->argument=arg;
    this->timeSlice=timeSlicee;
    this->stack=body != nullptr ? new uint64[STACK_SIZE] : nullptr;
    this->context={(uint64) &threadWrapper,
                   stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    };
    this->finished= false;
    this->sisNit=0;
    this->blokirana= false;
    if (this->body!= nullptr){
        Scheduler::put(this);
    }

/*:
            body(body),
            argument(arg)
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }*/
}
