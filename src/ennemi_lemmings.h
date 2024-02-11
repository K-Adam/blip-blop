#pragma once

#include "enemy.h"

#define LEM_SPEED 1

class EnnemiLemmings : public Ennemi {
   public:
	bool suicide;

	EnnemiLemmings();

	virtual void update();
	virtual void onAvance();
	virtual void onTombe();
	virtual void onMeure();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
