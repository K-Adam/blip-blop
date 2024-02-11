#pragma once

#include "event.h"

class EventMiFond : public Event {
   public:
	int id;
	int x;
	int y;

	virtual void doEvent();
};
