#pragma once

#include "level/events/event.h"

class EventEnemy : public Event {
   public:
	int x;
	int y;
	int enemy_id;
	int facing;

	void doEvent() override;
};
