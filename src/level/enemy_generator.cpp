#include "entities/sprite.h"
#include "level/enemy_generator.h"
#include "level/events/event_enemy.h"
#include "entities/ball.h"

#ifndef FACING_LEFT
#define FACING_LEFT 0
#define FACING_RIGHT 1
#endif

EnemyGenerator::EnemyGenerator() : t(0), destroyed(false) {}

void EnemyGenerator::update() {
	if (game_flag[FLAG_GEN_OFF] != 0) return;

	t += 1;
	t %= period;

	if (t == 0) {
		bool ok = true;

		for (Ball* s : list_players) {
			ok = (s->x < x - 100 || s->x > x + 100 || s->y < y - 100 || s->y > y + 100);
		}

		if (ok) {
			EventEnemy e;

			e.enemy_id = enemy_id;
			e.facing = facing;
			e.x = x;
			e.y = y;
			e.doEvent();
		}

		capacity -= 1;

		if (capacity <= 0) destroyed = true;
	}

	if (x < offset - 100) destroyed = true;
}
