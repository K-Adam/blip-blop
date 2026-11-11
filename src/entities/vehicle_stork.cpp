#include "entities/vehicle_stork.h"

namespace {
// The first stork of a batch drops its rider higher so two riders do not land on each other
struct StorkDrop {
	bool first = true;
};
}  // namespace

VehicleStork::VehicleStork() {
	pic = assets().pics.enemies.pic("red_car");
	hasBeenUsed = false;
	level().shared<StorkDrop>().first = true;
};

void VehicleStork::render() { Sprite::render(); }

void VehicleStork::updateUsed() {
	static const int SPEED = 8;

	can_be_used = false;
	x += SPEED;

	hero->dir = BBDIR_RIGHT;
	hero->weapon_dir = BBDIR_RIGHT;
	hero->x = x;
	hero->y = y - 20;

	if (x >= 1480) {
		bool& first = level().shared<StorkDrop>().first;
		if (first)
			hero->y = y - 80;
		else
			hero->y = y - 40;

		hero->unlockVehicle();
		hero = nullptr;
		hasBeenUsed = true;
		first = false;
	}
}

void VehicleStork::updateNotUsed() { x += 8; }
