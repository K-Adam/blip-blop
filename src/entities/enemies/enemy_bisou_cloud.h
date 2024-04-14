#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouCloud : public Enemy {
   public:
	int wait_shoot;
	int etape_shoot;

	EnemyBisouCloud();
	virtual void onNormal();
	virtual void update();
	virtual void onMeureWait();
	virtual void onMeureTombe();
	virtual void onMeureFin();
};
