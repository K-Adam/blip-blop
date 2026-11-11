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

	inline bool shouldActivate() override {
		if (cond == 0)
			return true;
		else if (cond == 1)
			return (level().rules.flags[flag] == val);
		else
			return (level().rules.flags[flag] >= val);
	};

	void doEvent() override {
		auto txt = std::make_unique<CoolText>();

		txt->ntxt = ntxt;

		level().entities.cool_texts.emplace_back(std::move(txt));
	};
};
