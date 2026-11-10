#include "entities/enemies/enemy_smurf_stork.h"
#include <cmath>
#include <numbers>

EnemySmurfStork::EnemySmurfStork() {
	pic = assets().pics.enemies.pic("smurf_stork");
	noCol();
	first = true;
}

void EnemySmurfStork::update() {
	if (first) {
		base_y = y;
		x -= 100;
		base_x = x;
		first = false;
	}

	if (level().rules.flags[1] < 5) return;

	x += 4;

	y = base_y + int(100 * std::sin((x - base_x) / 4 * std::numbers::pi / 180));

	if (x > level().camera.offset + 750 && level().rules.flags[0] < 3) {
		level().rules.flags[0] = 3;
	}
}
