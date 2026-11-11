#include "level/events/event_enemy_generator.h"
#include "level/enemy_generator_tmp.h"
#include "level/enemy_generator.h"

void EventEnemyGenerator::doEvent() {
	std::unique_ptr<EnemyGenerator> gen;

	if (tmp)
		gen = std::make_unique<EnemyGeneratorTMP>();
	else
		gen = std::make_unique<EnemyGenerator>();

	gen->enemy_id = enemy_id;
	gen->x = x;
	gen->y = y;
	gen->facing = facing;
	gen->period = period;
	gen->capacity = capacity;

	level().entities.enemy_gens.emplace_back(std::move(gen));
}
