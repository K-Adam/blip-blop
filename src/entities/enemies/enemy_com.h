#pragma once

#include "entities/enemies/enemy.h"

class EnnemiCOM : public Ennemi {
   public:
	EnnemiCOM();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
};
