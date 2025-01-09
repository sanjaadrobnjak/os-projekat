Implementation of a functional operating system kernel supporting multithreading with time-sharing. 
A memory allocator was implemented, and the kernel provides the concept of threads, semaphores, time-sharing support, 
as well as asynchronous context switching and preemption triggered by timer and keyboard interrupts.

The kernel is implemented as a "library-based" system, meaning that the user program (application) and the kernel share 
the same address space, forming a statically linked single program preloaded into the computer's main memory. Concurrent 
processes created within the application are essentially "lightweight" processes - threads running within the same program.

The kernel is implemented for the RISC-V processor architecture. The implementation uses the RISC-V assembler and the C and C++ 
programming languages. The implemented kernel is executed in a virtual environment—a RISC-V processor emulator.
