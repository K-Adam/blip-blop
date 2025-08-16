#pragma once

#include "entities/player.h"
#include "entities/shots/shot.h"

#include "core/constants.h"

class ShotBB : public Shot {
   public:
	Player* joueur;
	int dxReferentiel;

	ShotBB() : dxReferentiel(0){};
	int degats() const override = 0;
	int enflame() const override = 0;

	void aTue() override { joueur->addKilled(); };

	inline void aTouche(int pts) override {
		a_detruire = true;
		col_on = false;
		joueur->addPoints(pts);
	};

	void setDir(int d) override;
	// Exact direction in degrees counterclockwise from the right, the sprite takes the nearest of 16
	void setAngle(double deg, double speed);

   protected:
	void moveAlongAngle();

   private:
	double fx_ = 0, fy_ = 0, vx_ = 0, vy_ = 0;
};
