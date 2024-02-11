#pragma once

#include "enemy.h"

class EnnemiPikachuHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiPikachuHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
