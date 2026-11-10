#pragma once

#include "level/events/event.h"

class EventSetFlag : public Event {
   public:
	int flag;
	int val;

	void doEvent() override { level().rules.flags[flag] = val; };
};
