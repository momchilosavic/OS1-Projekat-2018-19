// File: PCB.h

#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "list.h"

typedef void (*SignalHandler)();
typedef unsigned SignalId;
enum State {CREATED, READY, RUNNING, BLOCKED, DONE};

class PCB{
	public:
		unsigned sp;
		unsigned ss;
		unsigned bp;

		unsigned isLimited;
		unsigned myTimeSlice;
		Thread* myThread;

		static int lastId;
		int myId;
		State myState;

		static unsigned listSize;
		static PCB** list;
		List* waitingToComplete;

		unsigned isPreempted;

		PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
		~PCB();
		void createStack();
	protected:
		static void wrapper();
		
	private:
		unsigned myStackSize;
		unsigned *myStack;

		static PCB** expandList(PCB** oldList, unsigned size);
};

#endif
