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

		level().entities.bonus_gens.emplace_back(gb);
	};
};
