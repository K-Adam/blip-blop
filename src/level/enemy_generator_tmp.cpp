#include "level/enemy_generator_tmp.h"
#include "level/events/event_enemy.h"

#ifndef FACING_LEFT
#define FACING_LEFT 0
#define FACING_RIGHT 1
#endif

void EnemyGeneratorTMP::update() {
	if (level().rules.flags[FLAG_GEN_OFF] != 0) return;

	t += 1;
	t %= period;

	if (t == 0) {
		EventEnemy e;

		e.enemy_id = enemy_id;
		e.facing = facing;
		e.x = x;
		e.y = y;
		e.doEvent();

		capacity -= 1;

		if (capacity <= 0) destroyed = true;
	}

	if ((facing == FACING_RIGHT && x < level().camera.offset - 200) || (facing == FACING_LEFT && x < level().camera.offset + 690)) destroyed = true;
}
