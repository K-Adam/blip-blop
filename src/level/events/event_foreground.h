#pragma once

#include "level/events/event.h"

class EventForeground : public Event {
   public:
	int x;
	int y;
	int bg_id;

	void doEvent() override;
};
