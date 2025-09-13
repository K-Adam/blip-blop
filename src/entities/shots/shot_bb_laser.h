#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLaser : public ShotBB {
   protected:
	int width_px;
	int hauteur;
	int base;

   public:
	ShotBBLaser();

	int getGiclureDir() const override { return dir; };

	inline int damage() const override { return 20; };

	inline int enflame() const override { return 0; };

	inline virtual void setEtape(int e) { anim_step = e; };

	inline void aTouche(int pts) override { hero->addPoints(pts); };

	inline void aTouche() override{};

	void render() override;
	void setDir(int d) override;
	void update() override;
	bool collision(const Sprite* s) const override;
};
