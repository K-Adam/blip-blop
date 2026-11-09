#pragma once

#include "level/events/event.h"

class EventVehicle : public Event {
   public:
	int vehicle_id;
	int x;
	int y;
	int dir;

	void doEvent() override;
};
