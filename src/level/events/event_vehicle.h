#pragma once

#include "level/events/event.h"

class EventVehicle : public Event {
   public:
	int id_vehicule;
	int x;
	int y;
	int dir;

	virtual void doEvent();
};
