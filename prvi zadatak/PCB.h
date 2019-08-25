// File: PCB.h

#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "list.h"
#include "PCBList.h"

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


		List* waitingToComplete;
		static PCBList* list;

		unsigned isPreempted;

		PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
		~PCB();
		void createStack();

		////////
		volatile static unsigned activePCBs;
		///////
	protected:
		static void wrapper();
		
	private:
		unsigned myStackSize;
		unsigned *myStack;

		static PCB** expandList(PCB** oldList, unsigned size);
};

#endif
