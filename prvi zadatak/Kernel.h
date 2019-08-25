// File: Kernel.h

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "PCB.h"
#include "Thread.h"
#include "list.h"

extern void tick();


class IdleThread : public Thread{
public:
	IdleThread();
	~IdleThread();
	void run();
};


class Kernel {
	public:
		static PCB* running;
		static PCB* loader;
		static PCB* idle;
		static IdleThread* idleThread;

		static volatile int alwaysTrue;
		static volatile int switch_on_demand;
		static volatile int lock;
		static volatile int switch_after_lock;
};

#endif
