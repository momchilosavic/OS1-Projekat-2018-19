// File: IVTEntry.h

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelE.h"


#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if (callOld == 1)\
		newEntry##numEntry.oldRoutine();\
}

typedef void interrupt(*pInterrupt)(...);


class IVTEntry{
	public:
		static IVTEntry *entries[];

		IVTNo ivtNo;
		KernelEv* kernelEv;

		pInterrupt oldRoutine;

		IVTEntry(IVTNo entryNo, pInterrupt newRoutine);
		~IVTEntry();

		void signal();
};



#endif /* IVTENTRY_H_ */
