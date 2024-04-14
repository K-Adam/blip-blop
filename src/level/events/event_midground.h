#pragma once

#include "level/events/event.h"

class EventMidground : public Event {
   public:
	int id;
	int x;
	int y;

	virtual void doEvent();
};
