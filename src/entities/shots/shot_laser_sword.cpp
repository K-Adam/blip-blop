#include "entities/shots/shot_laser_sword.h"
#include "entities/enemies/enemy_bisou_jedi.h"

ShotLaserSword::ShotLaserSword() : target(NULL), thrower(NULL), isdead(false), returning(false), accel(1) {
	lifespan = 200 + rnd() % 100;
	assets().sounds.level.play(13, SOUND_LOOP);
}

ShotLaserSword::~ShotLaserSword() { assets().sounds.level.stop(13); }

void ShotLaserSword::update() {
	step(4, 24);

	pic = assets().pics.enemies.anim("jedi_sword_spin")[anim_step];

	x += dx / 10;
	y += dy / 10;

	int xc = level().camera.offset - 400;
	int yc = 240;

	if (!isdead) {
		if (thrower->destroyed || ((Enemy*)thrower)->status == STATE_DYING) {
			isdead = true;
		}
	}

	if (isdead) {
		updateDestroyed();
	} else if (returning) {
		xc = thrower->x;
		yc = thrower->y - 25;

		int ddx = xc - x;
		int ddy = yc - y;

		if (ddx > -10 && ddx < 10 && ddy > -10 && ddy < 10) {
			destroyed = true;
			((EnemyBisouJedi*)thrower)->has_sword = true;
		}
	} else {
		lifespan -= 1;

		if (target->destroyed || lifespan < 0) {
			accel = 4;
			returning = true;
		} else {
			xc = target->x;
			yc = target->y - 15;
		}
	}

	if (xc > x && dx < 50)
		dx += accel;
	else if (dx > -50)
		dx -= accel;

	if (yc > y && dy < 50)
		dy += accel;
	else if (dy > -50)
		dy -= accel;

	colFromPic();
}
