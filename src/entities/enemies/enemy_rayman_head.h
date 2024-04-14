#pragma once

#include "entities/enemies/enemy.h"

class EnnemiTeteRayman : public Ennemi {
   private:
	int dx;

   public:
	// methodes "habituelles"
	EnnemiTeteRayman();
	virtual void update();
	virtual void onAvance();
	virtual void onMeure();

	virtual bool count() { return false; };

	virtual int degats();

	virtual void estTouche(Tir* tir);
};
