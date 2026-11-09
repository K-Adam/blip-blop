#pragma once

#include "level/events/event.h"

class EventEnemyGenerator : public Event {
   public:
	int x;
	int y;
	int enemy_id;
	int facing;
	int capacity;
	int period;
	bool tmp;

	void doEvent() override;
};
