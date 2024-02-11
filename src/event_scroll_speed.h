#pragma once

#include "event.h"

class EventScrollSpeed : public Event {
   public:
	int speed;

	virtual void doEvent() { scroll_speed = speed; };
};
