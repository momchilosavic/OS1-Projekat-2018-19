// File: event.cpp

#include "event.h"
#include "KernelE.h"
#include "Kernel.h"

Event::Event(IVTNo ivtNo){
	Kernel::lock = 1;
	myImpl = new KernelEv(ivtNo);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}

Event::~Event(){
	Kernel::lock = 1;
	delete myImpl;
	myImpl = 0;
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}

void Event::wait(){
	Kernel::lock = 1;
	myImpl->wait();
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}

void Event::signal(){
	Kernel::lock = 1;
	myImpl->signal();
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}
