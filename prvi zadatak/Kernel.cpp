// File: Kernel.cpp

#include "Kernel.h"
#include <dos.h>

PCB* Kernel::running = 0;
PCB* Kernel::loader = 0;
Thread* Kernel::idleThread = 0;
PCB* Kernel::idle = 0;

volatile int Kernel::lock = 0;
volatile int Kernel::alwaysTrue = 1;
volatile int Kernel::switch_on_demand = 0;

/* *** IDLE THREAD *** */

IdleThread::IdleThread():Thread(1024, 1){}

IdleThread::~IdleThread(){
	waitToComplete();
}

void IdleThread::run(){
	while(Kernel::alwaysTrue);
}

