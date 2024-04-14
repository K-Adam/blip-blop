#pragma once

#include "entities/enemies/enemy.h"

class EnnemiBulbizarreHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiBulbizarreHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
