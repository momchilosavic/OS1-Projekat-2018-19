// File: KernelE.cpp

#include "KernelE.h"
#include "Kernel.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"

extern void dispatch(); // funkcija dispatch je negde definisana

unsigned KernelEv::lastID = 0;
const unsigned KernelEv::listSize = 256;
KernelEv** KernelEv::list = new KernelEv*[KernelEv::listSize];

KernelEv::KernelEv(IVTNo ivtNo){
	id = ++lastID;
	this->ivtNo = ivtNo;
	value = 0;
	isBlocked = 0;
	owner = Kernel::running;
	IVTEntry::entries[ivtNo]->kernelEv = this;
	KernelEv::list[ivtNo] = this;
}

KernelEv::~KernelEv(){
	owner = 0;
	IVTEntry::entries[ivtNo]->kernelEv = 0;
}

void KernelEv::wait(){
	Kernel::lock = 1;
	if (owner == Kernel::running){
		if(value == 0){
			block();
			Kernel::lock = 0;
			isBlocked = 1;
			dispatch();
		}
		else
			value = 0;
	}
	else
		Kernel::lock = 0;
}

void KernelEv::signal(){
	Kernel::lock = 1;
	if(isBlocked == 0)
		value = 1;
	else{
		deblock();
		isBlocked = 0;
		Kernel::lock = 0;
		dispatch();
	}
	Kernel::lock = 0;
}

void KernelEv::block(){
	Kernel::running->myState = BLOCKED;
	value = 0;
}

void KernelEv::deblock(){
	owner->myState = READY;
	Scheduler::put(owner);
	value = 1;
}
