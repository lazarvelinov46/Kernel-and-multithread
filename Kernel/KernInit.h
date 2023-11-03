/*
 * KernInit.h
 *
 *  Created on: Jun 26, 2021
 *      Author: OS1
 */

#ifndef KERNINIT_H_
#define KERNINIT_H_


#include "KSem.h"
#include "Idle.h"

unsigned finished;
volatile int quantum=20;
unsigned oldTimerOFF, oldTimerSEG;
volatile int forcedContextSwitch= 0;
Idle* idle;

void init();
void restore();
void interrupt timer();


#endif /* KERNINIT_H_ */
