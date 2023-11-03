/*
 * Semaphore.cpp
 *
 *  Created on: May 24, 2021
 *      Author: OS1
 */

#include "KSem.h"
#include "Semaphor.h"
#include "Thread.h"

Semaphore::Semaphore(int init) {

	this->myImpl=new KernelSem(init);

}

Semaphore::~Semaphore() {

	if(this->myImpl)delete this->myImpl;

}

int Semaphore::wait(Time maxTimeToWait) {

	return this->myImpl->wait(maxTimeToWait);

}

void Semaphore::signal() {

	this->myImpl->signal();

}

int Semaphore::val() const {
	return this->myImpl->getValue();
}
