// File: KernelS.cpp

#include "KernelS.h"

#include <setjmp.h>
#include "SCHEDULE.H"
#include "Kernel.h"

#include <stdio.h>

unsigned KernelSem::lastID = 0;

KernelSemList* KernelSem::list = new KernelSemList();

KernelSem::KernelSem(int init) : value(init){
	blocked = new List();
	id = ++lastID;
	/*if(id > KernelSem::listSize){
			KernelSem::listSize <<= 1;
			KernelSem::list = expandList(KernelSem::list, KernelSem::listSize);
	}*/
	KernelSem::list->add(this);

	//printf("\nSEMAFOR USPESNO KREIRAN\tID:%i\tVAL: %i",id, value);
}

KernelSem::~KernelSem(){
	signal(value);
	delete blocked;
	blocked = 0;
	value = 0;

	KernelSem::list->remove(this);
	//printf("\nSEMAFOR USPESNO OBRISAN\tID:%i\tVAL: %i",id, value);
}

int KernelSem::signal(int n){
	if(n < 0)
		return n;
	if(n == 0){
		//printf("\nTHREAD ID: %d SIGNALED ON SEMAPHORE: %d\n", Thread::getRunningId(), id);
		if(value++ < 0)
			deblock();
		return 0;
	}
	else{
		//printf("\nTHREAD ID: %d SIGNALED ON SEMAPHORE: %d\n", Thread::getRunningId(), id);
		for(int i = 0; i < n; i++){
			if(value++ < 0)
				deblock();
		}
		int ret;
		if(value >= 0)
			ret = n - value;
		else
			ret = n;
		return ret;
	}
}

int KernelSem::wait(unsigned maxTimeToWait){
	//printf("\nTHREAD ID: %d WAIT ON SEMAPHORE: %d\n", Thread::getRunningId(), id);
	if(--value < 0)
		block(maxTimeToWait);
	int ret = Kernel::running->isPreempted;
	return ret;
}

int KernelSem::val() const{
	return value;
}

void KernelSem::block(unsigned maxTimeToWait){
	//printf("\n--- PCB: %d BLOCKED ON SEMAPHORE: %d ---\n", Thread::getRunningId(), id);
	Kernel::running->myState = BLOCKED;
	blocked->add(Kernel::running, maxTimeToWait);
	dispatch();
}

void KernelSem::deblock(){
	PCB* temp = blocked->popFirst();
	temp->myState = READY;
	Scheduler::put(temp);
	//printf("\n--- PCB: %d DEBLOCKED ON SEMAPHORE: %d ---\n", temp->myId, id);
}

KernelSem** KernelSem::expandList(KernelSem** oldList, unsigned size){
	KernelSem** newList = new KernelSem*[size];
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
