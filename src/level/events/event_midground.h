#pragma once

#include <string>

#include "level/events/event.h"

class EventMidground : public Event {
   public:
	std::string pic;  // Frame reference, see PictureBank::frame
	int x;
	int y;

	void doEvent() override;
};
