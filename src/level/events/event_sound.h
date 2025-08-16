#pragma once

#include "level/events/event.h"

class EventSound : public Event {
   public:
	int nsnd;

	void doEvent() override { sbk_niveau.play(nsnd); };
};
