#include <cstdlib>
#include "level/world.h"
#include "level/bonus_generator.h"
#include "entities/make_bonus.h"

BonusGenerator::BonusGenerator() : t(0), destroyed(false) {}

void BonusGenerator::update() {
	if (level().entities.bonus.size() < 3) {
		t += 1;
		t %= period;

		if (t == 0) {
			MakeBonus(level().camera.offset + 320 - 200 + rand() % 400, -50);
		}

		if (level().camera.offset > x + 500) destroyed = true;
	}
}
