/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */


#include "IVTEntry.h"

#include "KernelEv.h"

#include <dos.h>
#include <iostream.h>

IVTEntry* IVTEntry::entries[256]={0};

IVTEntry* IVTEntry::getEntry(IVTNo brojUlaza) {
	return IVTEntry::entries[brojUlaza];
}
/*
IVTEntry::IVTEntry(IVTNo ivtno) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	this->ivtno=ivtno;
	IVTEntry::entries[ivtno]=this;
#ifndef BCC_BLOCK_IGNORE
	this->oldInterrupt=getvect(ivtno);
#endif
	this->member=0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}
*/
IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	setvect(this->ivtno,this->oldInterrupt);
#endif
	IVTEntry::entries[this->ivtno]=0;
}

void IVTEntry::signalKernelEv() {
	if(this->member)this->member->signal();
}

void IVTEntry::callOldR() {
	if(this->oldInterrupt)(*(this->oldInterrupt))();
}

void IVTEntry::setKernelEvent(IVTNo brojUlaza,KernelEv* kev) {
	if(IVTEntry::entries[brojUlaza])IVTEntry::entries[brojUlaza]->member=kev;
}

IVTEntry::IVTEntry(IVTNo ivtno, functionPointer newInterrupt) {
	if(ivtno<0)ivtno=0;
	else if(ivtno>=256)ivtno=256;
	IVTEntry::entries[ivtno]=this;
#ifndef BCC_BLOCK_IGNORE
	this->oldInterrupt=getvect(ivtno);
	setvect(ivtno,newInterrupt);
#endif
	this->member=0;
}

void IVTEntry::changeEntry(functionPointer newInterrupt) {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtno,newInterrupt);
#endif
}
