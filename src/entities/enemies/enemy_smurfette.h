#pragma once

#include "entities/enemies/enemy.h"

class EnnemiSmurfette : public Ennemi {
   public:
	int etape_shoot;
	int wait_shoot;

	EnnemiSmurfette();

	virtual void update();
	virtual void onAvance();
	virtual void onTire();
	virtual void estTouche(Tir* tir);
	virtual void onCarbonise();
	virtual void onMeure();
};
