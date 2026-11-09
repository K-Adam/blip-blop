#pragma once

#include "level/events/event.h"
#include "core/debug.h"

class EventWeather : public Event {
   public:
	int intensity;
	int type;

	void doEvent() override {
		weather_intensity = intensity;
		weather_type = type;
	};
};
