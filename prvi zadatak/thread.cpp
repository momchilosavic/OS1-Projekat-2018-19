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
	Kernel::lock = 1;
	myPCB = new PCB(stackSize, timeSlice, this);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}

Thread::~Thread(){
}

void Thread::start(){
	Kernel::lock = 1;
	if(myPCB->myState != CREATED){
		Kernel::lock = 0;
		if(Kernel::switch_after_lock)
			dispatch();
		return;
	}
	
	myPCB->createStack();
	myPCB->myState = READY;
	Scheduler::put(myPCB);

	PCB::activePCBs++;
//		printf("\nACTIVE THREADS: %d\n", PCB::activePCBs);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();

}

void Thread::waitToComplete(){
	Kernel::lock = 1;
	if(this->myPCB == Kernel::running || this->myPCB->myState == DONE || this->myPCB->myState == CREATED || this->myPCB == Kernel::idle){
		Kernel::lock = 0;
		if(Kernel::switch_after_lock)
			dispatch();
		return;
	}
	
	this->myPCB->waitingToComplete->add(Kernel::running);
	Kernel::running->myState = BLOCKED;

	//printf("\n\tTHREAD %d WAITING ON THREAD %d\n", Thread::getRunningId(), getId());

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
	Thread* ret = 0;
	Kernel::lock = 1;
	PCB* r = PCB::list->find(id);
	if(r)
		ret = r->myThread;
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
	return ret;
}
/*Thread* Thread::getThreadById(ID id){
	Thread* ret = 0;
	Kernel::lock = 1;
	if(id <= PCB::lastId && id > 0 && PCB::list[id-1] != 0){
		ret = PCB::list[id-1]->myThread;
		Kernel::lock = 0;
		if(Kernel::switch_after_lock)
			dispatch();
		return ret;
	}
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
	return 0;
}*/

void dispatch(){
	Kernel::lock = 1;
		Kernel::switch_after_lock = 0;
		Kernel::switch_on_demand = 1;
	Kernel::lock = 0;
#ifndef BCC_BLOCK_IGNORE
	asm int 0x08;
#endif
}
