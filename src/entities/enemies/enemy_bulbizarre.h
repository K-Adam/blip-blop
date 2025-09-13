#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarre : public Enemy {
   public:
	EnemyBulbizarre();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
	virtual void onCarbonise();
	void takeHit(Shot* shot) override;
};
