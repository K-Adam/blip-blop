#pragma once

#include "enemy.h"

class EnnemiBisouBoom : public Ennemi
{
public:

	EnnemiBisouBoom();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir * tir);
	virtual void update();
};
