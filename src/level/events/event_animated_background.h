#pragma once

#include "level/events/event.h"

class EventAnimatedBackground : public Event {
   public:
	int x;
	int y;
	int bg_id;

	void doEvent() override;
};
