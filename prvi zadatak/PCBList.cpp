/*
 * PCBList.cpp
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#include "PCBList.h"
#include "PCB.h"
#include "KernelS.h"
#include "KernelE.h"

/* *** ELEMENT CLASS *** */

PCBElement::PCBElement(PCB* pcb){
	this->pcb = pcb;
	next = 0;
}

PCBElement::~PCBElement(){
	this->pcb = 0;
	next = 0;
}

/* *** LIST CLASS *** */

PCBList::PCBList() : head(0), tail(0), counter(0) {}

PCBList::~PCBList(){
	while(head)
		remove(head->pcb);
	head = 0;
	tail = 0;
}

int PCBList::count(){
	return counter;
}

void PCBList::add(PCB* pcb){
	if(head){
		tail->next = new PCBElement(pcb);
		tail = tail->next;
	}
	else{
		head = new PCBElement(pcb);
		tail = head;
		tail->next = 0;
		head->next = 0;
	}
	counter++;
}

void PCBList::remove(PCB* pcb){
	PCBElement * temp = head;
	PCBElement* prev = 0;
	while(temp && temp->pcb != pcb){
		prev = temp;
		temp = temp->next;
	}
	if(temp != 0){
		if(prev != 0 && temp->next != 0){
			prev->next = temp->next;
		}
		else{
			if(prev == 0){
				head = head->next;
			}
			if(temp->next == 0){
				tail = prev;
				if(tail)
					tail->next = 0;
			}
		}
		counter--;
		delete temp;
	}
}

PCB* PCBList::find(unsigned id){
	PCBElement* temp = head;
	while(temp && temp->pcb->myId != id)
		temp = temp->next;
	if(temp == 0)
		return 0;
	else
		return temp->pcb;
}


/* *** KS *** */
/* *** ELEMENT CLASS *** */

KernelSemElement::KernelSemElement(KernelSem* pcb){
	this->pcb = pcb;
	next = 0;
}

KernelSemElement::~KernelSemElement(){
	this->pcb = 0;
	next = 0;
}

/* *** LIST CLASS *** */

KernelSemList::KernelSemList() : head(0), tail(0), counter(0) {}

KernelSemList::~KernelSemList(){
	while(head)
		remove(head->pcb);
	head = 0;
	tail = 0;
}

int KernelSemList::count(){
	return counter;
}

void KernelSemList::add(KernelSem* pcb){
	if(head){
		tail->next = new KernelSemElement(pcb);
		tail = tail->next;
	}
	else{
		head = new KernelSemElement(pcb);
		tail = head;
		tail->next = 0;
		head->next = 0;
	}
	counter++;
}

void KernelSemList::remove(KernelSem* pcb){
	KernelSemElement * temp = head;
	KernelSemElement* prev = 0;
	while(temp && temp->pcb != pcb){
		prev = temp;
		temp = temp->next;
	}
	if(temp != 0){
		if(prev != 0 && temp->next != 0){
			prev->next = temp->next;
		}
		else{
			if(prev == 0){
				head = head->next;
			}
			if(temp->next == 0){
				tail = prev;
				if(tail)
					tail->next = 0;
			}
		}
		counter--;
		delete temp;
	}
}

KernelSem* KernelSemList::find(unsigned id){
	int i = 0;
	KernelSemElement* temp = head;
	while(temp && i < id){
		temp = temp->next;
		i++;
	}
	if(temp == 0)
		return 0;
	else
		return temp->pcb;
}




/* *** KE *** */
/* *** ELEMENT CLASS *** */

KernelEvElement::KernelEvElement(KernelEv* pcb){
	this->pcb = pcb;
	next = 0;
}

KernelEvElement::~KernelEvElement(){
	this->pcb = 0;
	next = 0;
}

/* *** LIST CLASS *** */

KernelEvList::KernelEvList() : head(0), tail(0), counter(0) {}

KernelEvList::~KernelEvList(){
	while(head)
		remove(head->pcb);
	head = 0;
	tail = 0;
}

int KernelEvList::count(){
	return counter;
}

void KernelEvList::add(KernelEv* pcb){
	if(head){
		tail->next = new KernelEvElement(pcb);
		tail = tail->next;
	}
	else{
		head = new KernelEvElement(pcb);
		tail = head;
		tail->next = 0;
		head->next = 0;
	}
	counter++;
}

void KernelEvList::remove(KernelEv* pcb){
	KernelEvElement * temp = head;
	KernelEvElement* prev = 0;
	while(temp && temp->pcb != pcb){
		prev = temp;
		temp = temp->next;
	}
	if(temp != 0){
		if(prev != 0 && temp->next != 0){
			prev->next = temp->next;
		}
		else{
			if(prev == 0){
				head = head->next;
			}
			if(temp->next == 0){
				tail = prev;
				if(tail)
					tail->next = 0;
			}
		}
		counter--;
		delete temp;
	}
}

KernelEv* KernelEvList::find(unsigned id){
	int i = 0;
	KernelEvElement* temp = head;
	while(temp && i < id){
		temp = temp->next;
		i++;
	}
	if(temp == 0)
		return 0;
	else
		return temp->pcb;
}
