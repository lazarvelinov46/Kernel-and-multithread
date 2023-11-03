/*
 * SemsList.h
 *
 *  Created on: Jun 26, 2021
 *      Author: OS1
 */

#ifndef SEMSLIST_H_
#define SEMSLIST_H_

#include "Semaphor.h"
#include "KSem.h"

class SemsList {
private:
	struct Node{
			KernelSem* sem;
			Node* next;
			Node(KernelSem* sem,Node* n=0):sem(sem),next(n){}
		};
	Node* head,*tail;
public:
	SemsList();
	virtual ~SemsList();
	void tickSemaphores();
	SemsList& addSem(KernelSem* sem);
	SemsList& removeSem(KernelSem* sem);
};

#endif /* SEMSLIST_H_ */
