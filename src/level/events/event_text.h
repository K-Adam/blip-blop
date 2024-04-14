#pragma once

#include "level/events/event.h"
#include "entities/cool_text.h"
#include "core/debug.h"

class EventText : public Event {
   public:
	int ntxt;
	int flag;
	int val;
	int cond;

	inline virtual bool aActiver() {
		if (cond == 0)
			return true;
		else if (cond == 1)
			return (game_flag[flag] == val);
		else
			return (game_flag[flag] >= val);
	};

	virtual void doEvent() {
		CoolText* txt = new CoolText();

		txt->ntxt = ntxt;

		list_txt_cool.emplace_back(txt);
	};
};
