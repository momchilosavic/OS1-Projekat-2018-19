// File: timer.cpp

#include "timer.h"
#include "lock.h"
#include "Kernel.h"
#include "SCHEDULE.h"
#include <dos.h>
#include "list.h"
#include "KernelS.h"
#include "KernelE.h"
#include "IVTEntry.h"

#include <stdio.h>

extern void tick();

pInterrupt Timer::oldISR = 0;
volatile unsigned int Timer::counter = 0;

void Timer::inic(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	Kernel::loader = new PCB(0, 2, 0);
	Kernel::loader->myState = RUNNING;
	Kernel::running = Kernel::loader;
	Kernel::idleThread = (Thread*)new IdleThread();
	Kernel::idle = new PCB(32, 1, Kernel::idleThread);
	Kernel::idle->createStack();
#ifndef BCC_BLOCK_IGNORE
	setvect(0x60, getvect(0x08));
	setvect(0x08, timer);

	UNLOCK
#endif
}

void Timer::restore(){
#ifndef BCC_BLOCK_IGNORE
	LOCK

	setvect(0x08, getvect(0x60));
#endif
	delete Kernel::idleThread;

	int i;
	for(i = 0; i < PCB::listSize; i++)
		delete PCB::list[i];
	delete[] PCB::list;

	for(i = 0; i < KernelSem::listSize; i++)
		delete KernelSem::list[i];
	delete[] KernelSem::list;

	for(i = 0; i < KernelEv::listSize; i++)
		delete KernelEv::list[i];
		delete IVTEntry::entries[i];
	delete[] KernelEv::list;
#ifndef BCC_BLOCK_IGNORE
	UNLOCK
#endif
}

unsigned int tsp;
unsigned int tss;
unsigned int tbp;
void interrupt Timer::timer(...){
	if(Kernel::switch_on_demand == 0){
		tick();
		for(int s = 0; s < KernelSem::listSize; s++)
			if(KernelSem::list[s] && KernelSem::list[s]->blocked)
				KernelSem::list[s]->blocked->notify(s);
		asm int 0x60;
		if(Kernel::lock == 0)
			counter--;
	}

	if(Kernel::lock == 1){
		Kernel::switch_on_demand = 1;
		return;
	}

	if(Kernel::switch_on_demand == 1 || counter == 0){

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		Kernel::running->sp = tsp;
		Kernel::running->ss = tss;
		Kernel::running->bp = tbp;

		//printf("\n==========\nSTARA NIT: %i\nSTATUS: %i\n============\n", Kernel::running->myId, Kernel::running->myState);

		if(Kernel::running->myState == RUNNING && Kernel::running != Kernel::idle){
			Kernel::running->myState = READY;
			Scheduler::put(Kernel::running);
		}
		if((Kernel::running = Scheduler::get()) == 0)
			Kernel::running = Kernel::idle;
		Kernel::running->myState = RUNNING;
		counter = Kernel::running->myTimeSlice;

		//printf("\n==========\nNOVA NIT: %i\n\tBROJAC: %i\n============\n", Kernel::running->myId, counter);

		tsp = Kernel::running->sp;
		tss = Kernel::running->ss;
		tbp = Kernel::running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif

		Kernel::switch_on_demand = 0;
	}
}
