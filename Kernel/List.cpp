/*
 * List.cpp
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#include "List.h"
#include <iostream.h>
#include "SCHEDULE.h"
#include "PCB.h"

List::List() {
	this->head=this->tail=0;
	this->semHead=this->semTail=0;
}

List& List::add(PCB* pcb) {
	Node* newPCB=new Node(pcb);
	if(!head){
		head=newPCB;
		tail=newPCB;
	}else{
		tail->next=newPCB;
		tail=tail->next;

	}
	return *this;
}

List& List::remove(ID id) {
	Node* current=head;
	Node* previous=0;
	while(current){
		if(current->pcb->getThread()->getId()==id){
			if(current==head){
				if(current==tail){
					head=tail=0;
					//cout<<"Izbrisana je nit: "<<current->pcb->getThread()->getId()<<endl;
					delete current;
				}
				else{
					head=head->next;
					//cout<<"Izbrisana je nit: "<<current->pcb->getThread()->getId()<<endl;
					delete current;
				}
			}else if(current==tail){
				tail=previous;
				tail->next=0;
				//cout<<"Izbrisana je nit: "<<current->pcb->getThread()->getId()<<endl;
				delete current;
			}else{
				previous->next=current->next;
				//cout<<"IZbrisana je nit: "<<current->pcb->getThread()->getId()<<endl;
				delete current;
			}
		}
		previous=current;
		current=current->next;
	}
	return *this;
}

PCB* List::getHead() {
	return head->pcb;
}

PCB* List::getTail() {
	return tail->pcb;
}

Thread* List::getThread(ID id) {
	Node* current=head;
	while(current){
		if(current->pcb->getThread()->getId()==id){
			return current->pcb->getThread();
		}
		current=current->next;
	}
	return 0;
}

/*
void List::ispis() {
	Node* current=head;
	if(head==0){
		cout<<"Nema";
	}else{
		while(current){
			cout<<current->pcb->getThread()->getId()<<"->";
			current=current->next;
		}
	}
	cout<<endl;
}
*/

void List::deleteBlockedThreads() {
	Node* current=head;
	while(current){
		current->pcb->setState(READY);
		Scheduler::put(current->pcb);
		Node* old=current;
		current=current->next;
		old->pcb=0;
		old->next=0;
		delete old;
	}
	head=tail=0;
}

List::~List() {
	Node* current=head;
		while(current){
			Node* old=current;
			current=current->next;
			old->pcb=0;
			old->next=0;
			delete old;
		}
	head=tail=0;
	unblockThreads();

}
//za semafore
List& List::addSemThread(int waitingTime) {

	SemNode* nod=new SemNode(PCB::running,waitingTime);
		if(!semHead){
			semHead=nod;
			semTail=nod;
			//cout<<"Dodata je nit broj "<<PCB::running->getThread()->getId()<<endl;
		}else{
			semTail->next=nod;
			semTail=semTail->next;
			//cout<<"Dodata je nit broj "<<PCB::running->getThread()->getId()<<endl;

		}
		return *this;
}

List& List::unblockThreads() {
	SemNode* current=semHead;
	if(current==0)return *this;
	while(current){
		if(current){
			current->pcb->setState(READY);
			Scheduler::put(current->pcb);
		}
		SemNode* old=current;
		current=current->next;
		old->pcb=0;
		old->next=0;
		delete old;
	}
	semHead=semTail=0;
	return *this;
}
List& List::unblockAThread() {
	//posto nije naglaseno kojim algoritmom da signal odblokira niti koristice se FIFO princip
	SemNode *toUnblock=semHead;
	if(toUnblock){
		toUnblock->pcb->setSignalized(YES);
		toUnblock->pcb->setState(READY);
		Scheduler::put(toUnblock->pcb);
	}else{
		return *this;
	}
	semHead=semHead->next;
	toUnblock->pcb=0;
	toUnblock->next=0;
	delete toUnblock;
	return *this;

}
int List::tickRedList() {
	int num=0;
	SemNode* temp=semHead;
	SemNode* prev=0;
	while(temp){
		if(temp->waitingTime==0){
			prev=temp;
			temp=temp->next;
		}else if(temp->waitingTime>0){
			temp->waitingTime--;
			if(temp->waitingTime==0){
				num++;
				temp->pcb->setSignalized(NO);
				if(prev==0){
					semHead=semHead->next;
					temp->pcb->setState(READY);
					Scheduler::put(temp->pcb);
					temp->pcb=0;
					temp->next=0;
					delete temp;
					temp=semHead;
				}else if(temp==semTail){
					prev->next=0;
					semTail=prev;
					temp->pcb->setState(READY);
					Scheduler::put(temp->pcb);
					temp->pcb=0;
					temp->next=0;
					delete temp;
					temp=0;
				}else{
					SemNode* old =temp;
					prev->next=temp->next;
					temp=temp->next;
					old->pcb->setState(READY);
					Scheduler::put(old->pcb);
					old->pcb=0;
					old->next=0;
					delete old;
				}
				//cout<<"Timeout"<<endl;
			}else{
				prev=temp;
				temp=temp->next;
			}
		}else{
			prev=temp;
			temp=temp->next;
		}
	}
	return num;
}

