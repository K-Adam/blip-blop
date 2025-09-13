#pragma once

#include "entities/enemies/enemy.h"

class EnemyRaymanHead : public Enemy {
   private:
	int dx;

   public:
	EnemyRaymanHead();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();

	bool count() override { return false; };

	int damage() override;

	void takeHit(Shot* shot) override;
};
