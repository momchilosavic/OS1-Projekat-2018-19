// File: semaphor.cpp

#include "semaphor.h"

Semaphore::Semaphore (int init){
	myImpl = new KernelSem(init);
};

Semaphore::~Semaphore (){
	delete myImpl;
	myImpl = 0;
};

int Semaphore::wait (Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
};

int Semaphore::signal(int n){
	return myImpl->signal(n);
};
		
int Semaphore::val() const{
	return myImpl->val();
}
