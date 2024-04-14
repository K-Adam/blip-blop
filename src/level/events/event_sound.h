#pragma once

#include "level/events/event.h"

class EventSound : public Event {
   public:
	int nsnd;

	virtual void doEvent() { sbk_niveau.play(nsnd); };
};
