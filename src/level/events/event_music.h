#pragma once

#include "level/events/event.h"

class EventMusic : public Event {
   public:
	int play;
	int id;

	void doEvent() override;
};
