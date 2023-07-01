/******************************************************************
 *
 *
 *		---------------
 *		  EventMusic.h
 *		---------------
 *
 *		Classe Evenement Music
 *
 *
 *		Prosper / LOADED -   2 Aout 2000
 *
 *
 *
 ******************************************************************/

#pragma once

#include "event.h"

class EventMusic : public Event {
   public:
	int play;
	int id;

	virtual void doEvent();
};
