// File: KernelE.cpp

#include "KernelE.h"
#include "Kernel.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"
#include <stdio.h>

extern void dispatch(); // funkcija dispatch je negde definisana

unsigned KernelEv::lastID = 0;
const unsigned KernelEv::listSize = 256;
KernelEvList* KernelEv::list = new KernelEvList();

KernelEv::KernelEv(IVTNo ivtNo){
	id = ++lastID;
	this->ivtNo = ivtNo;
	value = 0;
	isBlocked = 0;
	owner = Kernel::running;
	(IVTEntry::entries[ivtNo])->kernelEv = this;
	//KernelEv::list[ivtNo] = this;
	KernelEv::list->add(this);

	//printf("\nEVENT SEMAFOR USPESNO KREIRAN\t ID: %d\tULAZ: %d\tVAL: %d\n", owner->myId, ivtNo, value);

}

KernelEv::~KernelEv(){
	value = 0;

	//printf("\nEVENT SEMAFOR USPESNO OBRISAN\t ID: %d\tULAZ: %d\tVAL: %d\n", owner->myId, ivtNo, value);

	owner = 0;
	IVTEntry::entries[ivtNo]->kernelEv = 0;
	//KernelEv::list[ivtNo] = 0;
	KernelEv::list->remove(this);
}

void KernelEv::wait(){
	if (owner == Kernel::running){
		//Kernel::lock = 1;//
		if(value == 0){
			block();
			//Kernel::lock = 0;
			isBlocked = 1;
			dispatch();
		}
		else{
			value = 0;
			/*Kernel::lock = 0;
			if(Kernel::switch_after_lock)
				dispatch();*/
		}
	}
}

void KernelEv::signal(){
	//Kernel::lock = 1;
	if(isBlocked == 0)
		value = 1;
	else{
		deblock();
		isBlocked = 0;
		/*Kernel::lock = 0;
		if(Kernel::switch_after_lock)
			dispatch();*/
	}
	/*Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();*/
}

void KernelEv::block(){
	Kernel::running->myState = BLOCKED;
	value = 0;

	//printf("\nPCB %d BLOCKED ON EVENT NO %d", Thread::getRunningId(), ivtNo);
}

void KernelEv::deblock(){
	owner->myState = READY;
	Scheduler::put(owner);
	value = 1;

	//printf("\nPCB %d DEBLOCKED PCB %d ON EVENT NO %d", Thread::getRunningId(), owner->myId, ivtNo);

}
