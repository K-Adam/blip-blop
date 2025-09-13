#include "entities/vehicle_stork.h"

static bool first;

VehicleStork::VehicleStork() {
	pic = pbk_enemies[66];
	hasBeenUsed = false;
	first = true;
};

void VehicleStork::render() { Sprite::render(); }

void VehicleStork::updateUsed() {
	static const int bas_ailes[] = {0, 1, 2, 3, 2, 1};
	static const int SPEED = 8;

	can_be_used = false;
	x += SPEED;

	hero->dir = BBDIR_RIGHT;
	hero->dir_arme = BBDIR_RIGHT;
	hero->x = x;
	hero->y = y - 20;

	if (x >= 1480) {
		if (first)
			hero->y = y - 80;
		else
			hero->y = y - 40;

		hero->unlockVehicule();
		hero = NULL;
		hasBeenUsed = true;
		first = false;
	}
}

void VehicleStork::updateNotUsed() { x += 8; }
