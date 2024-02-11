#pragma once

#include "event.h"
#include "globals.h"

class EventFondAnime : public Event {
   public:
	int x;
	int y;
	int id_fond;

	virtual void doEvent();
};
