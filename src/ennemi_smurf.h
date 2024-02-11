#pragma once

#include "enemy.h"

class EnnemiSmurf : public Ennemi {
   public:
	int wait_for_jump;
	int jump_delay;
	int dy_saut;
	int speed;

	EnnemiSmurf();

	virtual void update();
	virtual void onAvance();
	virtual void onSaute();
	virtual void onMeure();
	virtual void onCarbonise();

	virtual void estTouche(Tir* tir);
};
