// File: semaphor.cpp

#include "semaphor.h"
#include "Kernel.h"

Semaphore::Semaphore (int init){
	Kernel::lock = 1;
	myImpl = new KernelSem(init);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
};

Semaphore::~Semaphore (){
	Kernel::lock = 1;
	delete myImpl;
	myImpl = 0;
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
};

int Semaphore::wait (Time maxTimeToWait){
	Kernel::lock = 1;
	return myImpl->wait(maxTimeToWait);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
};

int Semaphore::signal(int n){
	Kernel::lock = 1;
	return myImpl->signal(n);
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
};
		
int Semaphore::val() const{
	Kernel::lock = 1;
	return myImpl->val();
	Kernel::lock = 0;
	if(Kernel::switch_after_lock)
		dispatch();
}
