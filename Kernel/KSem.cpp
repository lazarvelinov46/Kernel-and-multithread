/*
 * KernelSem.cpp
 *
 *  Created on: May 24, 2021
 *      Author: OS1
 */

#include "KSem.h"

//#include "RedList.h"
#include "Thread.h"
#include <iostream.h>
#include "SemsList.h"

SemsList* KernelSem::allSemaphores=new SemsList();

KernelSem::KernelSem(int init) {
	this->value=init;
	this->redList=new List();
	allSemaphores->addSem(this);
}

int KernelSem::wait(Time waitingTime) {
	/*
	 * posto u zadatku pise maksimalno trajanje blokiranosti je waiting time
	 * to znaci da takva nit moze da se deblokira i pozivom metode signal
	 * takodje, posto apsolutna vrednost semafora, ukoliko je negativna
	 * treba da pokazuje broj blokiranih niti, niti se blokiraju samo ako je
	 * value negativno, i u varijanti wt=0 i wt>0
	 */
	if((--this->value)<0){
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif
		block(waitingTime);
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
		//pc blokirane niti je ovde
		if(PCB::running->getSignalized()==1){
			return 1;
		}else{
			return 0;
		}
	}else
	return 1;
}

void KernelSem::signal() {

	/*posto u zadatku pise da apsolutna vrednost semafora treba da bude jednaka
	 * broju blokiranih niti da li to znaci da svaki signal odblokira jednu nit?
	 */
	//da:
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(++this->value<=0){
		deblock();
	};
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

	//ne:
	/*
	if((++this->value)>=0){
		this->redList->unblockThreads();
	}
	*/
}

KernelSem::~KernelSem() {
	this->allSemaphores->removeSem(this);
	if(this->redList)delete this->redList;
}

int KernelSem::getValue() {
	return this->value;
}



SemsList* KernelSem::getAllSemaphores() {
	return KernelSem::allSemaphores;
}

void KernelSem::block(Time waitingTime) {
	PCB::running->setState(BLOCKED);
	redList->addSemThread(waitingTime);
	dispatch();
}

List* KernelSem::getRedList() {
	return this->redList;
}

void KernelSem::incValue(int inc) {
	this->value+=inc;
}

void KernelSem::deblock() {
	this->redList->unblockAThread();
}
