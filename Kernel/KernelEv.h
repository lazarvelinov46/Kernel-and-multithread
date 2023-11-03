/*
 * KernelEv.h
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

typedef unsigned char IVTNo;

#include "PCB.h"

class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo ivtno);
	virtual ~KernelEv();
	void wait ();
	void signal();
private:
	IVTNo ulaz;
	int value;
	PCB* eventCreator;
	void block();
	void deblock();
};

#endif /* KERNELEV_H_ */
