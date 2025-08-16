#pragma once

#include "entities/shots/shot.h"

class ShotCloud : public Shot {
   public:
	int etat;
	int etape_shoot;
	int delta_eclair;
	int eclair;
	int duree_vie;

	ShotCloud();
	void update() override;
	virtual void onCreation();
	virtual void onDestruction();
	virtual void onNormal();
	virtual void onTire();
	void affiche() override;
};
