#pragma once

#include "level/events/event.h"
#include "level/bonus_generator.h"

class EventGenBonus : public Event {
   public:
	int type;
	int periode;

	virtual void doEvent() {
		GenBonus* gb = new GenBonus();

		gb->periode = periode;
		gb->type = type;
		gb->x = x_activation;

		list_gen_bonus.emplace_back(gb);
	};
};
