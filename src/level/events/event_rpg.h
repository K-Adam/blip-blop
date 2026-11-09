#pragma once

#include "level/events/event.h"

class EventRPG : public Event {
   public:
	int num;
	int flag;
	int val;
	int cond;

	inline bool shouldActivate() override {
		if (cond == 0)
			return true;
		else
			return (game_flag[flag] == val);
	};

	void doEvent() override { rpg_to_play = num; };
};
