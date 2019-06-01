// File: KernelS.cpp

#include "KernelS.h"

#include <setjmp.h>
#include "SCHEDULE.H"
#include "Kernel.h"

#include <stdio.h>

unsigned KernelSem::lastID = 0;
unsigned KernelSem::listSize = 1;
KernelSem** KernelSem::list = new KernelSem*[1];

KernelSem::KernelSem(int init) : value(init){
	blocked = new List();
	Kernel::lock = 1;
	id = ++lastID;
	if(id > KernelSem::listSize){
			KernelSem::listSize <<= 1;
			KernelSem::list = expandList(KernelSem::list, KernelSem::listSize);
	}
	Kernel::lock = 0;
	KernelSem::list[id - 1] = this;
	if(Kernel::switch_on_demand == 1)
		dispatch();
	////////////////////////////////////////////
	//Kernel::lock = 1;
	//printf("\nSEMAFOR USPESNO KREIRAN\tID:%i\tVAL: %i",id, value);
	//Kernel::lock = 0;
	////////////////////////////////////////////
}

KernelSem::~KernelSem(){
	delete blocked;
	blocked = 0;
	value = 0;
	///////////////////////////////////////////
	//Kernel::lock = 1;
	//printf("\nSEMAFOR USPESNO OBRISAN\tID:%i\tVAL: %i",id, value);
	//Kernel::lock = 0;
	//////////////////////////////////////////
}

int KernelSem::signal(int n){
	if(n < 0)
		return n;
	if(n == 0){
		Kernel::lock = 1;
		if(value++ < 0)
			deblock();
		Kernel::lock = 0;
		return 0;
	}
	else{
		Kernel::lock = 1;
		for(int i = 0; i < n; i++){
			if(value++ < 0)
				deblock();
		}
		int ret;
		if(value >= 0)
			ret = n - value;
		else
			ret = n;
		Kernel::lock = 0;
		return ret;
	}
}

int KernelSem::wait(unsigned maxTimeToWait){
	Kernel::lock = 1;
	//////////////////////////////////////////
		//printf("\nSEMAFOR USPESNO ZAUSTAVLJEN\tID:%i\tVAL: %i", id,value);
		////////////////////////////////////////
	if(--value < 0)
		block(maxTimeToWait);
	int ret = Kernel::running->isPreempted;
	Kernel::lock = 0;
	return ret;
}

int KernelSem::val() const{
	return value;
}

void KernelSem::block(unsigned maxTimeToWait){
	////////////////////////
	//printf("\n\t\tBLOCK");
	Kernel::running->myState = BLOCKED;
	blocked->add(Kernel::running, maxTimeToWait);
	dispatch();
}

void KernelSem::deblock(){
	///////////////////
	//printf("\n\t\tDEBLOCK");
	PCB* temp = blocked->popFirst();
	temp->myState = READY;
	Scheduler::put(temp);
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
