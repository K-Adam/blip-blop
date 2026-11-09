#pragma once

#include "level/events/event.h"
#include "level/bonus_generator.h"

class EventBonusGenerator : public Event {
   public:
	int type;
	int period;

	void doEvent() override {
		BonusGenerator* gb = new BonusGenerator();

		gb->period = period;
		gb->type = type;
		gb->x = x_activation;

		list_gen_bonus.emplace_back(gb);
	};
};
