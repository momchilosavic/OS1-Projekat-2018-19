// File: thread.cpp

#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Kernel.h"
#include "list.h"
#include "Timer.h"

#include "lock.h"
#include <stdio.h>

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = new PCB(stackSize, timeSlice, this);

	////////////////////////////////////////////////
	//Kernel::lock=1;
	//printf("\n\tNIT USPESNO KREIRANA: %i\tstackSize: %i\ttimeSlice: %u", myPCB->myId,stackSize, myPCB->isLimited);
	//Kernel::lock=0;
	//////////////////////////////////////////////////
}

Thread::~Thread(){
}

void Thread::start(){
	if(myPCB->myState != CREATED)
		return;
	
	myPCB->createStack();
	myPCB->myState = READY;
	Scheduler::put(myPCB);

	//////////////////////////////////////
	//Kernel::lock=1;
	//printf("\n\tNIT USPESNO STARTOVANA\tPCB: %i", myPCB->id);
	//Kernel::lock=0;
	/////////////////////////////////////
}

void Thread::waitToComplete(){
	Kernel::lock = 1;
	if(this->myPCB == Kernel::running || this->myPCB->myState == DONE || this->myPCB->myState == CREATED || this->myPCB == Kernel::idle){
		Kernel::lock = 0;
		return;
	}
	
	this->myPCB->waitingToComplete->add(Kernel::running);
	Kernel::running->myState = BLOCKED;

	/////////////////////////////////////////////////
	//printf("\n\tNIT CEKA NA ZAVRSETAK\tPCB RUNNING: %i STATUS: %i\tPCB POZVANI: %i", Kernel::running->id, Kernel::running->state, myPCB->id);
	/////////////////////////////////////////////

	Kernel::lock = 0;
	dispatch();
}

ID Thread::getId(){
	if(this && this->myPCB)
		return myPCB->myId;
	return 0;
}

ID Thread::getRunningId(){
	return Kernel::running->myId;
}

Thread* Thread::getThreadById(ID id){
	if(id <= PCB::lastId && PCB::list[id-1] != 0)
		return PCB::list[id-1]->myThread;
	return 0;
}

void dispatch(){
	Kernel::lock = 1;

	///////////////////////////////////////////////////
	//printf("\n\tPOZVANA PROMENA KONTEKSTA\t RUNNING PCB: %i", Kernel::running->id);
	///////////////////////////////////////////////////////////////////////////

	Kernel::switch_on_demand = 1;
	Kernel::lock = 0;
	Timer::timer();
}
