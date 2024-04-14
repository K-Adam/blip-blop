#pragma once

#include "entities/enemies/enemy.h"

class EnnemiFlamecheHeros : public Ennemi {
   private:
	int dx;
	int dorkeball;

   public:
	EnnemiFlamecheHeros();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();
	virtual void estTouche(Tir* tir);
};
