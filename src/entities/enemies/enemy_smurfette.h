#pragma once

#include "entities/enemies/enemy.h"

class EnemySmurfette : public Enemy {
   public:
	int etape_shoot;
	int wait_shoot;

	EnemySmurfette();

	void update() override;
	virtual void onAvance();
	virtual void onTire();
	void takeHit(Shot* shot) override;
	virtual void onCarbonise();
	virtual void onMeure();
};
