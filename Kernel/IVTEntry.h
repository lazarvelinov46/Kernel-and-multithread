/*
 * IVTEntry.h
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "Thread.h"
#include "Event.h"



typedef void interrupt (*functionPointer)(...);



class IVTEntry {
public:
	static IVTEntry* getEntry(IVTNo brojUlaza);
	static void setKernelEvent(IVTNo brojUlaza,KernelEv* kev);
	//IVTEntry(IVTNo ivtno);
	IVTEntry(IVTNo ivtno,functionPointer newInterrupt);
	~IVTEntry();
	void signalKernelEv();
	void callOldR();
	void changeEntry(functionPointer newInterrupt);
private:
	static IVTEntry* entries[256];
	IVTNo ivtno;
	KernelEv* member;
	functionPointer oldInterrupt;
};


#endif /* IVTENTRY_H_ */
