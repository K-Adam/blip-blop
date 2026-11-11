#pragma once

#include "level/events/event.h"

class EventHoldFire : public Event {
   public:
	int flag;
	int val;

	void doEvent() override {
		level().rules.hold_fire.on = true;
		level().rules.hold_fire.flag = flag;
		level().rules.hold_fire.val = val;
	};
};
