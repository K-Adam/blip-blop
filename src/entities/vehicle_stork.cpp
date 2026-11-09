#include "entities/vehicle_stork.h"

static bool first;

VehicleStork::VehicleStork() {
	pic = pbk_enemies.pic("red_car");
	hasBeenUsed = false;
	first = true;
};

void VehicleStork::render() { Sprite::render(); }

void VehicleStork::updateUsed() {
	static const int wings_down[] = {0, 1, 2, 3, 2, 1};
	static const int SPEED = 8;

	can_be_used = false;
	x += SPEED;

	hero->dir = BBDIR_RIGHT;
	hero->weapon_dir = BBDIR_RIGHT;
	hero->x = x;
	hero->y = y - 20;

	if (x >= 1480) {
		if (first)
			hero->y = y - 80;
		else
			hero->y = y - 40;

		hero->unlockVehicle();
		hero = NULL;
		hasBeenUsed = true;
		first = false;
	}
}

void VehicleStork::updateNotUsed() { x += 8; }
