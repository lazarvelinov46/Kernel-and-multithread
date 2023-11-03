/*
 * List.h
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "Thread.h"
#include "PCB.h"

class List {
private:

	struct Node{
		PCB* pcb;
		Node* next;
		Node(PCB* pcb,Node* n=0):pcb(pcb),next(n){}
	};
	struct SemNode{
		PCB* pcb;
		Time waitingTime;
		SemNode* next;
		SemNode(PCB* pcb,Time wt,SemNode* next=0):pcb(pcb),waitingTime(wt),next(next){}
	};

	Node* head, *tail;
	SemNode* semHead,*semTail;
public:
	List();
	void ispis();
	List& add(PCB* pcb);
	List& remove(ID id);
	void deleteBlockedThreads();
	Thread* getThread(ID id);
	PCB* getHead();
	PCB* getTail();
	virtual ~List();
	//za semafore
	List& addSemThread(int waitingTime);
	List& unblockThreads();
	List& unblockAThread();

	int tickRedList();
};

#endif /* LIST_H_ */

