#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOM : public Enemy {
   public:
	EnemyCOM();
	void update() override;
	virtual void onAvance();
	virtual void onMeure();
};
