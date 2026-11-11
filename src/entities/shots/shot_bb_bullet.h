#pragma once

#include "entities/player.h"
#include "entities/shots/shot_bb.h"

class ShotBBBullet : public ShotBB {
   public:
	ShotBBBullet(int damage, const char* pic) : damage_(damage), pic_(pic) {}

	int damage() const override { return damage_; }
	int enflame() const override { return 0; }

	void update() override {
		moveAlongAngle();

		if (level().terrain.solid_wall(x, y) || x < level().camera.offset - 20 || x > level().camera.offset + 690 || y < -20 || y > 500) {
			pic = nullptr;
			noCol();
			destroyed = true;
		} else {
			pic = assets().pics.bb.pic(pic_);
			colAlongAngle();
		}
	}

	void render() override { drawAlongAngle(); }

   private:
	int damage_;
	const char* pic_;
};

// Named so Ball can hold default constructed pools of each
class ShotBBM16 : public ShotBBBullet {
   public:
	ShotBBM16() : ShotBBBullet(40, "shot_m16") {}
};

class ShotBBPM : public ShotBBBullet {
   public:
	ShotBBPM() : ShotBBBullet(75, "shot_pellet") {}
};
