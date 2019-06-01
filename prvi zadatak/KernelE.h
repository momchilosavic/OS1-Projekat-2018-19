// File: KernelE.h

#ifndef KERNELE_H_
#define KERNELE_H_

#include "PCB.h"
#include "Event.h"

class KernelEv{
	public:
		unsigned int id;
		static unsigned int lastID;

		KernelEv(IVTNo ivtNo);
		~KernelEv();

		void wait();
		void signal();

		static const unsigned listSize;
		static KernelEv** list;

	private:
		PCB* owner;
		int value;
		int isBlocked;
		IVTNo ivtNo;

		void block();
		void deblock();

};




#endif /* KERNELE_H_ */
