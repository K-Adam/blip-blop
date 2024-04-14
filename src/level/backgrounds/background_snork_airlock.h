#pragma once

#include "entities/sprite.h"

class BackgroundSnorkAirlock1 : public Sprite {
   private:
	int nb_joueur;

   public:
	static int etat;

	BackgroundSnorkAirlock1();
	virtual void update();
	virtual void affiche();
};

class BackgroundSnorkAirlock2 : public Sprite {
   public:
	BackgroundSnorkAirlock2();
	virtual void update();
	virtual void affiche();
};
