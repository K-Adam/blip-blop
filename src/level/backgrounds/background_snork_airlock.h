#pragma once

#include "entities/sprite.h"

class BackgroundSnorkAirlock1 : public Sprite {
   private:
	int nb_joueur;

   public:
	static int etat;

	BackgroundSnorkAirlock1();
	void update() override;
	void affiche() override;
};

class BackgroundSnorkAirlock2 : public Sprite {
   public:
	BackgroundSnorkAirlock2();
	void update() override;
	void affiche() override;
};
