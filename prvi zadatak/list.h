// File: list.h

#ifndef LIST_H_
#define LIST_H_

class PCB;

class Element{
	public:
		PCB* pcb;
		int sleepTime;
		Element* next;

		Element(PCB* pcb, int sleepTime);
		~Element();
};

class List{
public:
	List();
	~List();

	void add(PCB* pcb);
	void add(PCB* pcb, int sleepTime);
	PCB* popFirst();
	void notify(int id);

	int count();
private:
	Element* head;
	Element* headPreemptive;
	Element* tail;
	Element* tailNonPreemptive;
	int counter;
};


#endif /* LIST_H_ */
