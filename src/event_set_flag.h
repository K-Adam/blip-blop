#pragma once

#include "event.h"

class EventSetFlag : public Event {
   public:
	int flag;
	int val;

	virtual void doEvent() { game_flag[flag] = val; };
};
