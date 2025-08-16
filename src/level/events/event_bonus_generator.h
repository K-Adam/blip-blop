#pragma once

#include "level/events/event.h"
#include "level/bonus_generator.h"

class EventBonusGenerator : public Event {
   public:
	int type;
	int periode;

	void doEvent() override {
		BonusGenerator* gb = new BonusGenerator();

		gb->periode = periode;
		gb->type = type;
		gb->x = x_activation;

		list_gen_bonus.emplace_back(gb);
	};
};
