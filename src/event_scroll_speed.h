/******************************************************************
 *
 *
 *		----------------------
 *		  EventScrollSpeed.h
 *		----------------------
 *
 *		Classe Evenement Scroll Speed
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "event.h"

class EventScrollSpeed : public Event {
   public:
	int speed;

	virtual void doEvent() { scroll_speed = speed; };
};
