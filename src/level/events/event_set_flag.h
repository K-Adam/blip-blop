#pragma once

#include "level/events/event.h"

class EventSetFlag : public Event {
   public:
	int flag;
	int val;

	void doEvent() override { game_flag[flag] = val; };
};
