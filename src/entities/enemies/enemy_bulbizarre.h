#pragma once

#include "entities/enemies/enemy.h"

class EnemyBulbizarre : public Enemy {
   public:
	EnemyBulbizarre();
	void update() override;
	virtual void onWalk();
	virtual void onDie();
	virtual void onCharred();
	void takeHit(Shot* shot) override;
};
