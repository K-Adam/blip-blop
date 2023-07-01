#pragma once

#include "enemy.h"

class EnnemiFlameche : public Ennemi {
   public:
	EnnemiFlameche();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
