/*
 * KernInit.cpp
 *
 *  Created on: Jun 26, 2021
 *      Author: OS1
 */

#include "KernInit.h"
#include "KSem.h"
#include "Idle.h"
#include "Scheduler.h"
#include "SemsList.h"
#include <iostream.h>



extern void tick();

void init() {
#ifndef BCC_BLOCK_IGNORE
	asm{
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax


			mov word ptr es:0022h, seg timer
			mov word ptr es:0020h, offset timer


			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}


#endif
	idle=new Idle(defaultStackSize);
	idle->start();
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
#endif
}

unsigned tsp;
unsigned tss;
unsigned tbp;
void interrupt timer(){
	if(!forcedContextSwitch){
		tick();
		KernelSem::getAllSemaphores()->tickSemaphores();
		quantum--;
		//cout<<quantum<<" "<<Thread::getRunningId()<<endl;
	}

	if(quantum == 0  || forcedContextSwitch){
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp=tbp;
		//Ako running nije blocked ili finished znaci moze u scheduler
		if(PCB::running->getState()==READY
				&&PCB::running->getThread()!=idle){
			//cout<<"Raning stari: "<<Thread::getRunningId()<<endl;
			Scheduler::put(PCB::running);
		}
		PCB::running = Scheduler::get();

		//cout<<"Raning novi: "<<Thread::getRunningId()<<endl;

		if(PCB::running==0){
			//cout<<"Nema niti, ide idle"<<endl;
			PCB::running=idle->getMyPCB();
		}


		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
		quantum=PCB::running->getKvant();

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif

	}
	if(!forcedContextSwitch){


			#ifndef BCC_BLOCK_IGNORE
					asm int 60h;
			#endif
				}
	forcedContextSwitch=0;
}
