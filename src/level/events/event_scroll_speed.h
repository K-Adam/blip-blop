#pragma once

#include "level/events/event.h"

class EventScrollSpeed : public Event {
   public:
	int speed;

	void doEvent() override { level().camera.speed = speed; };
};
