//
// Created by os on 8/21/24.
//
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/scheduler.hpp"
#include "../h/print.hpp"

Thread::Thread(void (*body)(void *), void *arg) {
    int rez=thread_create(&myHandle, body, arg);
    if(rez!=0){
        printStringg("Greska pri kreiranju niti! \n");
    }
}

Thread::~Thread() {
    myHandle->setFinished(true);
    //delete myHandle;
    MemoryAllocator::mem_free(myHandle);
}

int Thread::start() {
    Scheduler::put(myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t) {
    return 0;
}

Thread::Thread() {
    int rez= thread_create(&myHandle, Thread::thread, (void*) this);
    if(rez!=0) {
        printStringg("Greska pri kreiranju niti!\n");
    }
}

void Thread::thread(void *p) {
    Thread* thr=(Thread*)p;
    if(thr) thr->run();
}

Semaphore::Semaphore(unsigned int init) {
    int rez= sem_open(&myHandle, init);
    if(rez!=0){
        printStringg("Greska pri kreiranju semafora!\n");
    }
}

Semaphore::~Semaphore() {
    int rez= sem_close(myHandle);
    if(rez!=0){
        printStringg("Greska pri brisanju semafora!\n");
    }
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::timedWait(time_t t) {
    return sem_timedwait(myHandle, t);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_tryWait(myHandle);
}

void PeriodicThread::terminate() {

}

PeriodicThread::PeriodicThread(time_t period) {
    this->period=period;
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
