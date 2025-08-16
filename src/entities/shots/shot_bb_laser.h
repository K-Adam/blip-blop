#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLaser : public ShotBB {
   protected:
	int largeur;
	int hauteur;
	int base;

   public:
	ShotBBLaser();

	int getGiclureDir() const override { return dir; };

	inline int degats() const override { return 20; };

	inline int enflame() const override { return 0; };

	inline virtual void setEtape(int e) { etape = e; };

	inline void aTouche(int pts) override { joueur->addPoints(pts); };

	inline void aTouche() override{};

	void affiche() override;
	void setDir(int d) override;
	void update() override;
	bool collision(const Sprite* s) const override;
};
