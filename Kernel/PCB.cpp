/*
 * PCB.cpp
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include "List.h"
#include <iostream.h>

PCB* PCB::running=0;


List* PCB::pcbList=new List();

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(this->blockedThreads){

			this->blockedThreads->deleteBlockedThreads();

			delete this->blockedThreads;
		}
	if(this->pcbThread)PCB::pcbList->remove(this->pcbThread->getId());

	if(this->stack)delete[] this->stack;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

}

Thread* PCB::getThread() {
	return this->pcbThread;
}
unsigned newsp;
unsigned newss;
PCB::PCB(Thread* t, StackSize stacksize, Time kvant):pcbThread(t),
		stackSize(stacksize),kvant(kvant){
	  this->stack = new unsigned[stackSize];
#ifndef BCC_BLOCK_IGNORE
	  newsp = FP_OFF(PCB::wrapper);
	  newss = FP_SEG(PCB::wrapper);
	  this->stack[stackSize-1]=0x200;
	  this->stack[stackSize-2]=newss;
	  this->stack[stackSize-3]=newsp;
	  this->ss=FP_SEG(stack+stackSize-12);
	  this->sp=FP_OFF(stack+stackSize-12);
#endif
	  this->bp=sp;
	  PCB::pcbList->add(this);
	  this->blockedThreads=new List();
	  this->state=BLOCKED;
	  this->signalized=NO;
}

void PCB::wrapper() {
	if(PCB::running->state==READY){

		//cout<<"Krece "<<Thread::getRunningId()<<endl;
		running->pcbThread->run();
		//cout<<"Gotovo "<<Thread::getRunningId()<<endl;

	}
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	running->state=FINISHED;
	running->getBlockedThreads()->deleteBlockedThreads();
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
	dispatch();
}

State PCB::getState() {
	return this->state;
}

void PCB::setState(State s) {
	if(s==BLOCKED||s==READY||s==FINISHED)
		this->state=s;
}

List* PCB::getBlockedThreads() {
	return this->blockedThreads;
}

List* PCB::getpcbList() {
	return PCB::pcbList;
}

Signalized PCB::getSignalized() {
	return this->signalized;
}

void PCB::setSignalized(Signalized sign) {
	if(sign==NO||sign==YES)
		this->signalized=sign;
}

Time PCB::getKvant() {
	return this->kvant;
}
