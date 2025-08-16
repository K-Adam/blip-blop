#pragma once

#include "level/events/event.h"

class EventEnemy : public Event {
   public:
	int x;
	int y;
	int id_ennemi;
	int sens;

	virtual void doEvent();
};
