#pragma once

#include "entities/enemies/enemy_smurf.h"

class EnemySmurfLazy : public EnemySmurf {
   public:
	EnemySmurfLazy() { dy_saut = -4; }

	inline virtual void affiche() { EnemySmurf::affiche(); }
};
