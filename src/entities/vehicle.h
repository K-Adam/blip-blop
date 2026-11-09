#pragma once

#include "entities/sprite.h"
#include "entities/ball.h"
#include "engine/controller.h"

class Vehicle : public Sprite {
   public:
	bool can_be_used;

	Ball* hero;
	Controller* ctrl;

	Vehicle();

	void update() override;
	virtual Ball* seekRider();

	virtual bool canFire() = 0;
	virtual bool canChangeDir() = 0;
	virtual void updateUsed() = 0;
	virtual void updateNotUsed() = 0;
};
