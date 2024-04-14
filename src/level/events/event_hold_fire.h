#pragma once

#include "level/events/event.h"

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
