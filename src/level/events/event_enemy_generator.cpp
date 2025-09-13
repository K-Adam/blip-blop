#include "level/events/event_enemy_generator.h"
#include "level/enemy_generator_tmp.h"
#include "level/enemy_generator.h"

void EventEnemyGenerator::doEvent() {
	EnemyGenerator* gen;

	if (tmp)
		gen = new EnemyGeneratorTMP();
	else
		gen = new EnemyGenerator();

	gen->enemy_id = enemy_id;
	gen->x = x;
	gen->y = y;
	gen->facing = facing;
	gen->period = period;
	gen->capacity = capacity;

	list_enemy_gens.emplace_back(gen);
}
