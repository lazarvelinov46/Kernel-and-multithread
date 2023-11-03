/*
 * KernelSem.h
 *
 *  Created on: May 24, 2021
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

#include "List.h"
#include "Semaphor.h"
class SemsList;

class KernelSem {
public:
	int wait(Time waitingTime);
	void signal();
	KernelSem(int init);
	virtual ~KernelSem();
	int getValue();
	static SemsList* getAllSemaphores();
	List* getRedList();
	void incValue(int inc);
private:
	int value;
	List* redList;
	void block(Time waitingTime);
	void deblock();
	static SemsList* allSemaphores;
};

#endif /* KSEM_H_ */
