/*
 * PCBList.h
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

class PCB;

class PCBElement{
	public:
		PCB* pcb;
		PCBElement* next;

		PCBElement(PCB* pcb);
		~PCBElement();
};

class PCBList{
	public:
		PCBList();
		~PCBList();
		void add(PCB* pcb);
		void remove(PCB* pcb);
		PCB* find(unsigned id);
		int count();

	private:
		PCBElement* head;
		PCBElement* tail;
		int counter;
};

// -------------------------------------------- //


class KernelSem;

class KernelSemElement{
	public:
		KernelSem* pcb;
		KernelSemElement* next;

		KernelSemElement(KernelSem* pcb);
		~KernelSemElement();
};

class KernelSemList{
	public:
		KernelSemList();
		~KernelSemList();
		void add(KernelSem* pcb);
		void remove(KernelSem* pcb);
		KernelSem* find(unsigned id);
		int count();

	private:
		KernelSemElement* head;
		KernelSemElement* tail;
		int counter;
};


// ------------------------------------- //

class KernelEv;

class KernelEvElement{
	public:
		KernelEv* pcb;
		KernelEvElement* next;

		KernelEvElement(KernelEv* pcb);
		~KernelEvElement();
};

class KernelEvList{
	public:
		KernelEvList();
		~KernelEvList();
		void add(KernelEv* pcb);
		void remove(KernelEv* pcb);
		KernelEv* find(unsigned id);
		int count();

	private:
		KernelEvElement* head;
		KernelEvElement* tail;
		int counter;
};

#endif /* PCBLIST_H_ */
