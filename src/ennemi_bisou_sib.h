#pragma once

#include "enemy.h"

class EnnemiBisouSIB : public Ennemi {
   public:
	int encaisse;

	EnnemiBisouSIB();

	virtual void update();
	virtual void estTouche(Tir* tir);
};
