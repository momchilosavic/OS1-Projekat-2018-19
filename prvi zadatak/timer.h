// File: timer.h
#ifndef _TIMER_H_
#define _TIMER_H_

#include <dos.h>

typedef void interrupt (*pInterrupt)(...);

class Timer{
	public:
		static volatile unsigned int counter;
		static volatile int flag;

		static pInterrupt oldISR;

		static void inic();
		static void restore();
		static void interrupt timer(...);
};

#endif
