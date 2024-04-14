#pragma once

#include "entities/sprite.h"

class FondSnorkSas1 : public Sprite {
   private:
	int nb_joueur;

   public:
	static int etat;

	FondSnorkSas1();
	virtual void update();
	virtual void affiche();
};

class FondSnorkSas2 : public Sprite {
   public:
	FondSnorkSas2();
	virtual void update();
	virtual void affiche();
};
