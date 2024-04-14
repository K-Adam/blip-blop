#pragma once

#include "level/events/event.h"
#include "core/globals.h"

class EventForeground : public Event {
   public:
	int x;
	int y;
	int id_fond;

	virtual void doEvent();
};
