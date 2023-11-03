/*
 * Event.h
 *
 *  Created on: Jun 21, 2021
 *      Author: OS1
 */

// File: event.h
#ifndef _event_h_
#define _event_h_


class KernelEv;
class IVTEntry;

/*
#define PREPAREENTRY(ivtNo, oldRoutine)\
void interrupt inter##ivtNo(...);\
IVTEntry newEntry##ivtNo(ivtNo);\
newEntry##ivtNo.changeEntry(inter##ivtNo);\
void interrupt inter##ivtNo(...){\
newEntry##ivtNo.signalKernelEv();\
if(oldRoutine==1)\
newEntry##ivtNo.callOldInterrupt();\
}\
*/

#define PREPAREENTRY(ivtNo, oldRoutine)\
void interrupt inter##ivtNo(...); \
IVTEntry newEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) {\
newEntry##ivtNo.signalKernelEv();\
if (oldRoutine == 1)\
newEntry##ivtNo.callOldR();\
}\



typedef unsigned char IVTNo;

class Event {
public:
	 Event (IVTNo ivtNo);
	 ~Event ();
	 void wait ();
protected:
	 friend class KernelEv;
	 void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};
#endif
