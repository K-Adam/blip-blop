#pragma once

#include "entities/enemies/enemy.h"

class EnemyCOM : public Enemy {
   public:
	EnemyCOM();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
};
