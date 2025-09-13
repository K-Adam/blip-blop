#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouStar : public Enemy {
   public:
	int wait_shoot;
	int etape_shoot;

	EnemyBisouStar();
	virtual void onAvance();
	virtual void onTire();
	virtual void onMeureEntier();
	virtual void onMeureHaut();
	virtual void onMeureHautFin();
	virtual void onCarbonise();
	void takeHit(Shot* shot) override;
	void update() override;
};
