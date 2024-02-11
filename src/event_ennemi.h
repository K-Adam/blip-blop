#pragma once

#include "event.h"
#include "globals.h"

class EventEnnemi : public Event {
   public:
	int x;
	int y;
	int id_ennemi;
	int sens;

	virtual void doEvent();
};
