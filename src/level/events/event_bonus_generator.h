#pragma once

#include "level/events/event.h"
#include "level/bonus_generator.h"

class EventBonusGenerator : public Event {
   public:
	int type;
	int period;

	void doEvent() override {
		auto gb = std::make_unique<BonusGenerator>();

		gb->period = period;
		gb->type = type;
		gb->x = x_activation;

		level().entities.bonus_gens.emplace_back(std::move(gb));
	};
};
