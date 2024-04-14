#pragma once

#include "level/events/event.h"
#include "core/debug.h"

class EventWeather : public Event {
   public:
	int intensite;
	int type;

	virtual void doEvent() {
		intensite_meteo = intensite;
		type_meteo = type;
	};
};
