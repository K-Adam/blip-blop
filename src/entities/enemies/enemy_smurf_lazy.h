#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfLazy : public EnemySmurf {
   public:
	EnemySmurfLazy() { dy_jump = -4; }
};
