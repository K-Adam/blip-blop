#pragma once

#include "entities/player.h"
#include "entities/shots/shot.h"

#include "core/constants.h"

class ShotBB : public Shot {
   public:
	Player* joueur;
	int dxReferentiel;

	ShotBB() : dxReferentiel(0){};
	virtual int degats() const = 0;
	virtual int enflame() const = 0;

	virtual void aTue() { joueur->addKilled(); };

	inline virtual void aTouche(int pts) {
		a_detruire = true;
		col_on = false;
		joueur->addPoints(pts);
	};

	virtual void setDir(int d);
	// Exact direction in degrees counterclockwise from the right, the sprite takes the nearest of 16
	void setAngle(double deg, double speed);

   protected:
	void moveAlongAngle();

   private:
	double fx_ = 0, fy_ = 0, vx_ = 0, vy_ = 0;
};
