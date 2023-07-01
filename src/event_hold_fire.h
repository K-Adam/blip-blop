/******************************************************************
 *
 *
 *		-------------------
 *		  EventHoldFire.h
 *		-------------------
 *
 *		Classe Evenement Hold Fire
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "event.h"

class EventHoldFire : public Event {
   public:
	int flag;
	int val;

	virtual void doEvent() {
		hold_fire = true;
		flag_hold_fire = flag;
		val_hold_fire = val;
	};
};
