#pragma once

#include "enemy.h"

class EnnemiBisouCoeur : public Ennemi {
   public:
	EnnemiBisouCoeur();
	virtual void onAvance();
	virtual void onMeure();
	virtual void onPart();
	virtual void estTouche(Tir* tir);
	virtual void update();
};
