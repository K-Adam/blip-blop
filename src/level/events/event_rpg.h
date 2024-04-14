#pragma once

#include "level/events/event.h"

class EventRPG : public Event {
   public:
	int num;
	int flag;
	int val;
	int cond;

	inline virtual bool aActiver() {
		if (cond == 0)
			return true;
		else
			return (game_flag[flag] == val);
	};

	virtual void doEvent() { rpg_to_play = num; };
};
