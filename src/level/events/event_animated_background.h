#pragma once

#include "level/events/event.h"

class EventAnimatedBackground : public Event {
   public:
	int x;
	int y;
	int id_fond;

	void doEvent() override;
};
