#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBLaser : public ShotBB {
   public:
	int getSplatterDir() const override { return dir; };

	inline int damage() const override { return 20; };

	inline int enflame() const override { return 0; };

	inline void onHit(int pts) override { hero->addPoints(pts); };

	inline void onHit() override{};

	// Degrees counterclockwise from the right, the beam runs from x, y + 3 to the first wall
	void aim(double angle);
	void render() override;
	void update() override;
	bool collision(const Sprite* s) const override;

   private:
	double ux_ = 1;
	double uy_ = 0;
	double angle_ = 0;
	int length_ = 0;
};
