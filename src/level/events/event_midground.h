#pragma once

#include "level/events/event.h"

class EventMiFond : public Event {
   public:
	int id;
	int x;
	int y;

	virtual void doEvent();
};
