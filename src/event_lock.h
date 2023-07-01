/******************************************************************
 *
 *
 *		---------------
 *		  EventLock.h
 *		---------------
 *
 *		Classe Evenement Lock
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "event.h"

class EventLock : public Event {
   public:
	int cond;
	int flag;
	int val;

	virtual void doEvent();
};
