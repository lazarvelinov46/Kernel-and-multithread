/*
 * main.cpp
 *
 *  Created on: May 21, 2021
 *      Author: OS1
 */




#include "PCB.h"
#include "List.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "Semaphor.h"
#include "Event.h"
#include "IVTEntry.h"
#include "user.h"
//#include "KernInit.h"
#include "Thread.h"
#include <iostream.h>

extern void init();
extern void restore();
extern int userMain (int argc, char* argv[]);

PCB* mainT=new PCB(0,defaultStackSize,defaultTimeSlice);


int syncPrintf(const char *format, ...)  {
	int res;
	va_list args;
#ifndef BCC_BLOCK_IGNORE
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
#endif
	return res;
}


int main(int argc, char * argv[]) {

	init();
	PCB::running=mainT;
	int ret=userMain(argc,argv);
	PCB::running=mainT;
	restore();

}
