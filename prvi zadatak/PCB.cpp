// File: PCB.cpp

#include "PCB.h"
#include "Kernel.h"
#include "SCHEDULE.h"
#include <dos.h>
#include <STDIO.H>

/*unsigned PCB::listSize = 4;
PCB** PCB::list =  new PCB*[4];*/
PCBList* PCB::list = new PCBList();
int PCB::lastId = 0;
/////
volatile unsigned PCB::activePCBs = 0;
////

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread){
	this->myStackSize = stackSize;
	this->myTimeSlice = timeSlice;
	if(timeSlice > 0)
		this->isLimited = 1;
	else
		this->isLimited = 0;
	this->myThread = myThread;

	this->myState = CREATED;

	this->myId = ++PCB::lastId;
	PCB::list->add(this);
	/*if(myId > PCB::listSize){
		PCB::listSize <<= 1;
		PCB::list = PCB::expandList(PCB::list, PCB::listSize);
	}*/
	//printf("\nPCB CREATED: %d\n", myId);
	//PCB::list[this->myId-1] = this;
	this->waitingToComplete = new List();

	this->myStack = 0;
	this->sp = 0;
	this->ss = 0;
	this->bp = 0;
	this->isPreempted = 0;
}

PCB::~PCB(){
	Kernel::lock = 1;
	this->myThread = 0;
	if(this->myStack){
		delete[] this->myStack;
		/*Kernel::lock = 1;
			printf("\n\tSTACK SUCCESSFULLY DELETED:\tID: %d\tSIZE: %d", myId, myStackSize);
			Kernel::lock = 0;
			if(Kernel::switch_after_lock)
				dispatch();*/
		this->myStack = 0;
	}
	PCB::list->remove(this);
	delete this->waitingToComplete;
	this->waitingToComplete = 0;
	//printf("\nPCB REMOVED: %d\n", this->myId);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}

void PCB::createStack(){
	if(this->myStackSize > 65536L)
		this->myStackSize = 65536;
	
	this->myStackSize /= sizeof(unsigned);
	
	this->myStack = new unsigned[this->myStackSize];
#ifndef BCC_BLOCK_IGNORE
	this->myStack[this->myStackSize - 1] = FP_SEG(this->myThread);
	this->myStack[this->myStackSize - 2] = FP_OFF(this->myThread);

	this->myStack[this->myStackSize - 5] = 0x200;
	
	this->myStack[this->myStackSize - 6] = FP_SEG(PCB::wrapper);
	this->myStack[this->myStackSize - 7] = FP_OFF(PCB::wrapper);
	
	this->ss = FP_SEG(this->myStack + this->myStackSize - 16);
	this->sp = FP_OFF(this->myStack + this->myStackSize - 16);
	this->bp = this->sp;
#endif

		//printf("\n\tSTACK SUCCESSFULLY CREATED:\tID: %d\tSIZE: %d", myId, myStackSize);
}

PCB** PCB::expandList(PCB** oldList, unsigned size){
	PCB** newList = new PCB*[size];
	int i;
	for(i = 0 ; i < (size>>1); i++){
		newList[i] = oldList[i];
		oldList[i] = 0;
	}
	for(i = size>>1; i<size; i++)
		newList[i] = 0;

	delete[] oldList;
	oldList = 0;

	return newList;
}

void PCB::wrapper(){
	Kernel::running->myThread->run();

	Kernel::lock = 1;
	PCB::activePCBs--;
	////////////////
		//printf("\n*** THREAD DONE: %d ***\n\t*** ACTIVE THREADS:%d ***\n", Thread::getRunningId(), PCB::activePCBs);
		//////////////////
	PCB* wakeUp;
	while(wakeUp = Kernel::running->waitingToComplete->popFirst()){
		wakeUp->myState = READY;
		Scheduler::put(wakeUp);
		//printf("\n\t\tPCB %d WOKE UP PCB %d\n", Thread::getRunningId(), wakeUp->myId);
	}

	Kernel::running->myState = DONE;
	Kernel::lock = 0;
	dispatch();
}
