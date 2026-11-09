#pragma once

#include "level/world.h"

class Event {
   public:
	virtual ~Event() = default;
	int x_activation;

	inline virtual bool shouldWake() { return (x_activation <= offset); };

	inline virtual bool shouldActivate() { return true; };

	virtual void doEvent() = 0;
};
