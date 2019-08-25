// File: Kernel.cpp

#include "Kernel.h"
#include <dos.h>
#include <stdio.h>

PCB* Kernel::running = 0;
PCB* Kernel::loader = 0;
IdleThread* Kernel::idleThread = 0;
PCB* Kernel::idle = 0;

volatile int Kernel::lock = 0;
volatile int Kernel::alwaysTrue = 1;
volatile int Kernel::switch_on_demand = 0;
volatile int Kernel::switch_after_lock = 0;

/* *** IDLE THREAD *** */

IdleThread::IdleThread():Thread(4096, 1){}

IdleThread::~IdleThread(){
}

void IdleThread::run(){
	Kernel::lock = 1;
	while(PCB::activePCBs > 1){
		Kernel::lock = 0;
		if(Kernel::switch_after_lock)
			dispatch();
		Kernel::lock = 0;
	}
}

