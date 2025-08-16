#pragma once

#include "level/world.h"

class Event {
   public:
	virtual ~Event() = default;
	int x_activation;

	inline virtual bool aReveiller() { return (x_activation <= offset); };

	inline virtual bool aActiver() { return true; };

	virtual void doEvent() = 0;
};
