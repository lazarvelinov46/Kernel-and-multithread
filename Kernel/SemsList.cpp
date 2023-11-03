/*
 * SemsList.cpp
 *
 *  Created on: Jun 26, 2021
 *      Author: OS1
 */

#include "SemsList.h"

SemsList::SemsList() {
	head=tail=0;

}

SemsList::~SemsList() {
	Node* current=head;
			while(current){
				Node* old=current;
				current=current->next;
				old->sem=0;
				old->next=0;
				delete old;
			}
		head=tail=0;
}

SemsList& SemsList::addSem(KernelSem* sem) {
	if(head==0){
		head=tail=new Node(sem);
	}else{
		tail=tail->next=new Node(sem);
	}
	return *this;
}

void SemsList::tickSemaphores() {
	Node* current=head;
	while(current){
		int inc=current->sem->getRedList()->tickRedList();
		current->sem->incValue(inc);
		current=current->next;
	}
}

SemsList& SemsList::removeSem(KernelSem* sem) {
	Node* current=head;
	Node* prev=0;
	while(current){
		if(current->sem==sem){
			if(current==head){
				head=head->next;
				current->next=0;
				current->sem=0;
				current=0;
				delete current;
				return *this;
			}else if(current==tail){
				tail=prev;
				tail->next=0;
				current->sem=0;
				current=0;
				delete current;
				return *this;
			}else{
				prev->next=current->next;
				current->next=0;
				current->sem=0;
				current =0;
				delete current;
				return *this;
			}
		}
		prev=current;
		current=current->next;
	}
	return *this;
}
