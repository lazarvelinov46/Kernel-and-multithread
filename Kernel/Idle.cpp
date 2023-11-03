/*
 * Idle.cpp
 *
 *  Created on: Jun 27, 2021
 *      Author: OS1
 */

#include "Idle.h"
#include "Thread.h"
#include "PCB.h"

Idle::Idle(StackSize sz):Thread(sz,1) {

}


void Idle::run() {
	for(;;){}
}

void Idle::start() {
	this->getMyPCB()->setState(READY);
}

Idle::~Idle() {
	// TODO Auto-generated destructor stub
}

