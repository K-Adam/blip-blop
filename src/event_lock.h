#pragma once

#include "event.h"

class EventLock : public Event {
   public:
	int cond;
	int flag;
	int val;

	virtual void doEvent();
};
