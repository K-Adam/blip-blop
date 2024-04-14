#include <cstdlib>
#include "core/globals.h"
#include "level/bonus_generator.h"
#include "entities/make_bonus.h"

BonusGenerator::BonusGenerator() : t(0), a_detruire(false) {}

void BonusGenerator::update() {
	if (list_bonus.size() < 3) {
		t += 1;
		t %= periode;

		if (t == 0) {
			MakeBonus(offset + 320 - 200 + rand() % 400, -50);
		}

		if (offset > x + 500) a_detruire = true;
	}
}
