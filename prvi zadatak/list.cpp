// File: list.cpp

#include "list.h"
#include "PCB.h"
#include "KernelS.h"
#include "SCHEDULE.h"

List::List() : head(0), headPreemptive(0), tail(0), tailNonPreemptive(0), counter(0) {}

List::~List(){
	while(head)
		popFirst();
	head = 0;
	headPreemptive = 0;
	tail = 0;
	tailNonPreemptive = 0;
}

int List::count(){
	return counter;
}

void List::add(PCB* pcb){
	if(pcb == 0)
		return;

	Element* newElement = new Element(pcb, 0);

	if(head){
		tail->next = newElement;
		tail = tail->next;
	}
	else{
		head = newElement;
		tail = head;
	}
}

void List::add(PCB* pcb, int sleepTime){
	if(pcb == 0)
		return;

	Element* newElement = new Element(pcb, sleepTime);

	if(sleepTime == 0){
		if(head == 0){
			head = newElement;
			tail = head;
			tailNonPreemptive = head;
		}
		else{
			if(tailNonPreemptive == 0){
				tailNonPreemptive = newElement;
				tailNonPreemptive->next = head;
				head = tailNonPreemptive;
			}
			else{
				tailNonPreemptive->next = newElement;
				tailNonPreemptive = tailNonPreemptive->next;
				tailNonPreemptive->next = headPreemptive;
				if(headPreemptive == 0)
					tail = tailNonPreemptive;
			}
		}
	}
	else{
		if(head == 0){
			head = newElement;
			tail = newElement;
			headPreemptive = newElement;
		}
		else{
			if(headPreemptive == 0){
				headPreemptive = newElement;
				tail->next = headPreemptive;
				tail = headPreemptive;
			}
			else{
				Element* temp = headPreemptive;
				Element* prev = tailNonPreemptive;
				while(temp && temp->sleepTime <= newElement->sleepTime){
					newElement->sleepTime -= temp->sleepTime;
					prev = temp;
					temp = temp->next;
				}
				if(!temp)
					prev->next = newElement;
				else{
					if(!prev){
						newElement->next = temp;
						head = newElement;
						headPreemptive = newElement;
					}
					else{
						prev->next = newElement;
						newElement->next = temp;
						if(prev == tailNonPreemptive)
							headPreemptive = newElement;
					}
					temp->sleepTime -= newElement->sleepTime;
				}
			}
		}
	}
	counter++;
}

PCB* List::popFirst(){
	if(head == 0)
		return 0;

	Element* ret = head;
	head = head->next;
	if(head == 0){
		tail = 0;
		headPreemptive = 0;
		tailNonPreemptive = 0;
	}
	else{
		if(ret == tailNonPreemptive)
			tailNonPreemptive = 0;
		else
			if(ret == headPreemptive)
				headPreemptive = head;
	}

	counter--;
	ret->pcb->isPreempted = 1;
	PCB* r = ret->pcb;
	delete ret;
	return r;
}

void List::notify(int id){
	if(headPreemptive == 0)
		return;

	Element* temp = headPreemptive;

	headPreemptive->sleepTime--;
	while(headPreemptive->sleepTime == 0){
		Scheduler::put(headPreemptive->pcb);
		headPreemptive->pcb->myState = READY;
		(KernelSem::list->find(id))->value += 1;
		headPreemptive->pcb->isPreempted = 0;
		headPreemptive = headPreemptive->next;
		if(tailNonPreemptive == 0)
			head = headPreemptive;
		else{
			tailNonPreemptive->next = headPreemptive;
			if(headPreemptive == 0)
				tail = tailNonPreemptive;
		}

		if(head == 0){
			tail = 0;
			headPreemptive = 0;
			tailNonPreemptive = 0;
		}
		counter--;
	}
}
/* *** ELEMENT CLASS *** */

Element::Element(PCB* pcb, int sleepTime){
	this->pcb = pcb;
	this->sleepTime = sleepTime;
	next = 0;
}

Element::~Element(){
	this->pcb = 0;
	this->sleepTime = 0;
	next = 0;
}


