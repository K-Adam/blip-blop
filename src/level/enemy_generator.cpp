#include "entities/sprite.h"
#include "level/enemy_generator.h"
#include "level/events/event_enemy.h"
#include "entities/ball.h"

EnemyGenerator::EnemyGenerator() : t(0), destroyed(false) {}

void EnemyGenerator::update() {
	if (level().rules.flags[FLAG_GEN_OFF] != 0) return;

	t += 1;
	t %= period;

	if (t == 0) {
		bool ok = true;

		for (Ball* s : session().players) {
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

	if (x < level().camera.offset - 100) destroyed = true;
}
