/*
 * KernelEv.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */

#include "KernelEv.h"
#include "IVTEntry.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include <iostream.h>

KernelEv::KernelEv(IVTNo ivtno) {
	this->ulaz=ivtno;
	this->value=0;
	this->eventCreator=PCB::running;
	IVTEntry::setKernelEvent(ivtno,this);
}

KernelEv::~KernelEv() {
	IVTEntry::setKernelEvent(this->ulaz,0);
}

void KernelEv::wait() {
	if(this->eventCreator==PCB::running){
		if(this->value==0){

			block();

		}else this->value=0;
	}

}

void KernelEv::signal() {
	if(this->value==0){

		deblock();

	}else return;


	dispatch();

}

void KernelEv::block() {
	this->eventCreator->setState(BLOCKED);
	dispatch();
}

void KernelEv::deblock() {
	this->value=1;
	this->eventCreator->setState(READY);
	Scheduler::put(this->eventCreator);
}
