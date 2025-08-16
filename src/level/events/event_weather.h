#pragma once

#include "level/events/event.h"
#include "core/debug.h"

class EventWeather : public Event {
   public:
	int intensite;
	int type;

	void doEvent() override {
		intensite_meteo = intensite;
		type_meteo = type;
	};
};
