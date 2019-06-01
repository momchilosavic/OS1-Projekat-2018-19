// File: KernelS.h

#ifndef _KERNELS_H_
#define _KERNELS_H_

#include "list.h"

class KernelSem{
	public:
		unsigned id;
		static unsigned lastID;

		KernelSem(int init);
		~KernelSem();

		int wait (unsigned maxTimeToWait);
		int signal(int n = 0);
		int val () const; 

		static KernelSem** list;
		static unsigned listSize;

		List* blocked;

		int value;

	private:

		void block(unsigned maxTimeToWait);
		void deblock();

		KernelSem** expandList(KernelSem** oldList, unsigned size);
};

#endif
