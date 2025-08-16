#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouSky : public Enemy {
   public:
	int wait_for_pisser;

	EnemyBisouSky();
	virtual void onAvance();
	virtual void onTire();
	virtual void onMeureEntier();
	virtual void onMeureHaut();
	virtual void onMeureHautFin();
	virtual void onCarbonise();
	void estTouche(Shot* tir) override;
	void update() override;
};
