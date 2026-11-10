#pragma once

#include "level/events/event.h"
#include "core/debug.h"

class EventWeather : public Event {
   public:
	int intensity;
	int type;

	void doEvent() override {
		level().rules.weather_intensity = intensity;
		level().rules.weather_type = type;
	};
};
