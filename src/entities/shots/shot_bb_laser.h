#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLaser : public ShotBB {
   protected:
	int width_px;
	int height_px;
	int base;

   public:
	ShotBBLaser();

	int getSplatterDir() const override { return dir; };

	inline int damage() const override { return 20; };

	inline int enflame() const override { return 0; };

	inline virtual void setAnimStep(int e) { anim_step = e; };

	inline void onHit(int pts) override { hero->addPoints(pts); };

	inline void onHit() override{};

	void render() override;
	void setDir(int d) override;
	void update() override;
	bool collision(const Sprite* s) const override;
};
