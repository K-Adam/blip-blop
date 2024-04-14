#include "entities/shots/shot_cake.h"
#include "core/debug.h"

ShotCake::ShotCake() { pic = pbk_ennemis[194]; }

void ShotCake::update() {
	lat_grav += 1;
	lat_grav %= LATENCE_GRAVITE;

	if (lat_grav == 0 && dy < GRAVITE_MAX) dy += 1;

	y += dy;

	x += dx;

	if (mur_opaque(x, y) || x < offset - 100 || x > offset + 700 || y > 520 || y < -50) a_detruire = true;

	colFromPic();
}
