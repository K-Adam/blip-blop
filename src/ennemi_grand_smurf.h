#pragma once

#include "enemy.h"

class EnnemiGrandSmurf : public Ennemi {
   public:
	Sprite* cible;
	int wait_change_cible;
	int dx;
	int attak_wanted;

	EnnemiGrandSmurf();
	virtual void updateCible();
	virtual void update();
	virtual void updateChoice();
	virtual void onAvance();
	virtual void onAtata();
	//	virtual void onMeure();
};
