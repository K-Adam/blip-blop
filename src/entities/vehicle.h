#pragma once

#include "entities/sprite.h"
#include "entities/ball.h"
#include "engine/controller.h"

class Vehicle : public Sprite {
   public:
	bool can_be_used;

	Ball* joueur;
	Controller* ctrl;

	Vehicle();

	virtual void update();
	virtual Ball* scanne();

	virtual bool canFire() = 0;
	virtual bool canChangeDir() = 0;
	virtual void updateUsed() = 0;
	virtual void updateNotUsed() = 0;
};
