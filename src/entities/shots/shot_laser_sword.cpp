#include "entities/shots/shot_laser_sword.h"
#include "entities/enemies/enemy_bisou_jedi.h"

ShotLaserSword::ShotLaserSword() : isdead(false), cible(NULL), retour(false), lanceur(NULL), accel(1) {
	duree_vie = 200 + rand() % 100;
	sbk_level.play(13, SOUND_LOOP);
}

ShotLaserSword::~ShotLaserSword() { sbk_level.stop(13); }

void ShotLaserSword::update() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 24;
	}

	pic = pbk_enemies[525 + anim_step];

	x += dx / 10;
	y += dy / 10;

	int xc = offset - 400;
	int yc = 240;

	if (!isdead) {
		if (lanceur->destroyed || ((Enemy*)lanceur)->status == ETAT_MEURE) {
			isdead = true;
		}
	}

	if (isdead) {
		updateDestroyed();
	} else if (retour) {
		xc = lanceur->x;
		yc = lanceur->y - 25;

		int ddx = xc - x;
		int ddy = yc - y;

		if (ddx > -10 && ddx < 10 && ddy > -10 && ddy < 10) {
			destroyed = true;
			((EnemyBisouJedi*)lanceur)->a_epee = true;
		}
	} else {
		duree_vie -= 1;

		if (cible->destroyed || duree_vie < 0) {
			accel = 4;
			retour = true;
		} else {
			xc = cible->x;
			yc = cible->y - 15;
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
