/*
 * Thread.cpp
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "List.h"
#include "SCHEDULE.h"
#include "Idle.h"
#include <iostream.h>

extern volatile int forcedContextSwitch;
extern void interrupt timer();
extern Idle* idle;
ID Thread::sId=0;


Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(this->myPCB)delete this->myPCB;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif

	this->myPCB->setState(READY);
	Scheduler::put(myPCB);

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void Thread::waitToComplete() {

#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(PCB::running->getState()==FINISHED ||
			this->myPCB->getState()==FINISHED||
			(PCB::running==this->myPCB)||
			this->myPCB==idle){
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
		return;
	}

	//cout<<"Ja sam nit "<<Thread::getRunningId()<<" cekam nit "<<this->getId()<<endl;
	PCB::running->setState(BLOCKED);
	this->myPCB->getBlockedThreads()->add(PCB::running);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
	dispatch();


}

ID Thread::getId() {
	return this->id;
}

ID Thread::getRunningId() {
	return PCB::running->getThread()->getId();
}

Thread* Thread::getThreadById(ID id) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	return PCB::getpcbList()->getThread(id);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

Thread::Thread(StackSize stacksize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(stacksize>maxStackSize)
		stacksize=defaultStackSize;
	stacksize/=sizeof(unsigned);
	this->id=Thread::sId++;
	myPCB = new PCB((Thread*)this,stacksize,timeSlice);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

}

void dispatch() {
	//cout<<"U dispecu sam "<<Thread::getRunningId()<<endl;
#ifndef BCC_BLOCK_IGNORE
	lock;
	forcedContextSwitch=1;
	timer();
	unlock;
#endif

}

PCB* Thread::getMyPCB() {
	return this->myPCB;
}

