/******************************************************************
 *
 *
 *		---------------
 *		  EventMiFond.h
 *		---------------
 *
 *		Classe Evenement MiFond
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "event.h"

class EventMiFond : public Event {
   public:
	int id;
	int x;
	int y;

	virtual void doEvent();
};
