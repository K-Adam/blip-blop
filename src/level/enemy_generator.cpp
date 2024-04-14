#include "entities/sprite.h"
#include "level/enemy_generator.h"
#include "level/events/event_enemy.h"
#include "entities/ball.h"

#ifndef SENS_GAUCHE
#define SENS_GAUCHE 0
#define SENS_DROITE 1
#endif

EnemyGenerator::EnemyGenerator() : t(0), a_detruire(false) {}

void EnemyGenerator::update() {
	if (game_flag[FLAG_GEN_OFF] != 0) return;

	t += 1;
	t %= periode;

	if (t == 0) {
		bool ok = true;

		for (Ball* s : list_joueurs) {
			ok = (s->x < x - 100 || s->x > x + 100 || s->y < y - 100 || s->y > y + 100);
		}

		if (ok) {
			EventEnemy e;

			e.id_ennemi = id_ennemi;
			e.sens = sens;
			e.x = x;
			e.y = y;
			e.doEvent();
		}

		capacite -= 1;

		if (capacite <= 0) a_detruire = true;
	}

	if (x < offset - 100) a_detruire = true;
}
