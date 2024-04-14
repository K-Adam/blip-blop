#pragma once

#include "entities/enemies/enemy_lemmings.h"

class EnemyLemmingsFlying : public EnemyLemmings {
   public:
	EnemyLemmingsFlying() { y = -5; };
};
