#pragma once

#include <string>

#include "level/events/event.h"

class EventForeground : public Event {
   public:
	int x;
	int y;
	std::string pic;  // Frame reference, see PictureBank::frame

	void doEvent() override;
};
