#pragma once

#include "entities/enemies/enemy.h"

class EnnemiPic : public Ennemi {
   private:
	int y_origine;

   public:
	EnnemiPic();
	virtual void update();
	virtual void affiche();
	virtual void colFromPic();
	virtual void estTouche(Tir* tir);
	virtual int degats() { return 5; };

	virtual bool count() { return false; };
};
