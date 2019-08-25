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
#include "PCB.h"
#include "user.h"
#include <stdio.h>

extern void tick();

pInterrupt Timer::oldISR = 0;
volatile unsigned int Timer::counter = 0;

void Timer::inic(){
#ifndef BCC_BLOCK_IGNORE
	LOCK
#endif
	Kernel::loader = new PCB(4096, 2, 0);
	Kernel::loader->createStack();
	Kernel::loader->myState = RUNNING;

	Kernel::idleThread = new IdleThread();
	Kernel::idleThread->start();

	Kernel::running = Kernel::loader;
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
	delete PCB::list;

	delete KernelSem::list;

	delete KernelEv::list;
	delete[] IVTEntry::entries;
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
		for(int s = 0; s < KernelSem::list->count(); s++)
			if(KernelSem::list->find(s) && KernelSem::list->find(s)->blocked){
				KernelSem::list->find(s)->blocked->notify(s);
			}
		asm int 0x60;
		if(counter > 0)
			counter--;
	}

	if(Kernel::lock == 1){
		if((Kernel::running->isLimited && counter == 0) || Kernel::switch_on_demand == 1)
			Kernel::switch_after_lock = 1;
		return;
	}

	if(Kernel::switch_on_demand == 1 || (Kernel::running->isLimited && counter == 0)){

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

		if(Kernel::running->myState == RUNNING){
			Kernel::running->myState = READY;
			Scheduler::put(Kernel::running);
		}
		Kernel::running = Scheduler::get();
		Kernel::running->myState = RUNNING;
		counter = Kernel::running->myTimeSlice;


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
