#pragma once

#include "enemy.h"

class EnnemiComHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiComHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
