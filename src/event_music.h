#pragma once

#include "event.h"

class EventMusic : public Event {
   public:
	int play;
	int id;

	virtual void doEvent();
};
