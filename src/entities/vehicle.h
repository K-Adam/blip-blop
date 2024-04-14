#pragma once

#include "entities/sprite.h"
#include "entities/ball.h"
#include "engine/controller.h"

class Vehicule : public Sprite {
   public:
	bool can_be_used;

	Couille* joueur;
	Controlor* ctrl;

	Vehicule();

	virtual void update();
	virtual Couille* scanne();

	virtual bool canFire() = 0;
	virtual bool canChangeDir() = 0;
	virtual void updateUsed() = 0;
	virtual void updateNotUsed() = 0;
};
