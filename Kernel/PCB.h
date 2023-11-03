/*
 * PCB.h
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"
#include <dos.h>
#define lock asm{\
	pushf;\
	cli;\
}
#define unlock asm popf;
typedef int State;
const State BLOCKED =0;
const State READY=1;
const State FINISHED=2;

typedef int Signalized;
const Signalized NO=0;
const Signalized YES=1;


class PCB {
private:
	Thread* pcbThread;
	unsigned* stack;
	StackSize stackSize;
	Time kvant;
	State state;
	Signalized signalized;
	static List* pcbList;
	List* blockedThreads;
public:

	unsigned ss;
	unsigned sp;
	unsigned bp;
	Thread* getThread();
	PCB(Thread* t, StackSize stackSize=defaultStackSize, Time kvant=defaultTimeSlice);
	List* getBlockedThreads();

	static List* getpcbList();
	static PCB* running;
	static void wrapper();
	Signalized getSignalized();
	void setSignalized(Signalized sign);
	State getState();
	Time getKvant();
	void setState(State s);
	virtual ~PCB();
};

#endif /* PCB_H_ */
