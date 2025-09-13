#pragma once

#include "entities/enemies/enemy.h"

class EnemyBisouBoom : public Enemy {
   public:
	EnemyBisouBoom();
	virtual void onAvance();
	virtual void onMeure();
	void takeHit(Shot* shot) override;
	void update() override;
};
