#pragma once

#include "level/events/event.h"

class EventVehicule : public Event {
   public:
	int id_vehicule;
	int x;
	int y;
	int dir;

	virtual void doEvent();
};
