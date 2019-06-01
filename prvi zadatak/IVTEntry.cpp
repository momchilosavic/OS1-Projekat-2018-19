// File: IVTEntry.cpp

#include "IVTEntry.h"
#include <dos.h>

IVTEntry* IVTEntry::entries[256]={0};

IVTEntry::IVTEntry(IVTNo entryNo, pInterrupt newRoutine){
	ivtNo = entryNo;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif
	kernelEv = 0;
	entries[ivtNo] = this;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
}

void IVTEntry::signal(){
	if(kernelEv)
		kernelEv->signal();
}
