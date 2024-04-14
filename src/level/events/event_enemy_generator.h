#pragma once

#include "level/events/event.h"

class EventGenEnnemi : public Event {
   public:
	int x;
	int y;
	int id_ennemi;
	int sens;
	int capacite;
	int periode;
	bool tmp;

	virtual void doEvent();
};
