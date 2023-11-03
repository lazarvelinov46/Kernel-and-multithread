/*
 * Idle.h
 *
 *  Created on: Jun 27, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle:public Thread {
public:
	void run();
	void start();
	Idle(StackSize sz);
	virtual ~Idle();
};

#endif /* IDLE_H_ */
