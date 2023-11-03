/*
 * Thread.h
 *
 *  Created on: May 18, 2021
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_


#define lock asm{\
	pushf;\
	cli;\
}\


#define unlock asm popf

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
const StackSize maxStackSize = 65535L;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB; // Kernel's implementation of a user's thread
class List;
class Thread {
public:
	 void start();
	 void waitToComplete();
	 virtual ~Thread();

	 ID getId();
	 PCB* getMyPCB();
	 static ID getRunningId();
	 static Thread * getThreadById(ID id);

protected:
	 friend class PCB;
	 virtual void run() {}
	 Thread (StackSize stacksize = defaultStackSize, Time timeSlice = defaultTimeSlice);
private:
	 PCB* myPCB;
	 static ID sId;
	 ID id;
};
void dispatch ();
#endif
