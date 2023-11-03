/*
 * Event.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"


Event::Event(IVTNo ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	this->myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif

}

Event::~Event() {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(myImpl)delete this->myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
