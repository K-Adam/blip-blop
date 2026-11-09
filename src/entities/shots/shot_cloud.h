#pragma once

#include "entities/shots/shot.h"

class ShotCloud : public Shot {
   public:
	int status;
	int shoot_step;
	int lightning_timer;
	int lightning;
	int lifespan;

	ShotCloud();
	void update() override;
	virtual void onCreation();
	virtual void onDestruction();
	virtual void onNormal();
	virtual void onShoot();
	void render() override;
};
