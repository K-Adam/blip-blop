#include "entities/enemies/enemy_smurf_stork.h"
#include "core/math_utils.h"

EnemySmurfStork::EnemySmurfStork() {
	pic = pbk_ennemis[355];
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

	if (game_flag[1] < 5) return;

	x += 4;

	y = base_y + sini(100, ((x - base_x) / 4) % 360);

	if (x > offset + 750 && game_flag[0] < 3) {
		game_flag[0] = 3;
	}
}
