// File: semaphor.h

#ifndef _semaphor_h_
#define _semaphor_h_

#include "KernelS.h"

typedef unsigned int Time;

class Semaphore {
	public:
		Semaphore (int init=1);
		virtual ~Semaphore ();
		virtual int wait (Time maxTimeToWait);
		virtual int signal(int n = 0);
		int val () const; // Returns the current value of the semaphore
	private:
		KernelSem* myImpl;
};

#endif
