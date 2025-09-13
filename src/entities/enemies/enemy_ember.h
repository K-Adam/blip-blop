#pragma once

#include "entities/enemies/enemy.h"

class EnemyEmber : public Enemy {
   public:
	EnemyEmber();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
};
