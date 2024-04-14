#include "level/events/event_enemy_generator.h"
#include "level/enemy_generator_tmp.h"
#include "level/enemy_generator.h"

void EventGenEnnemi::doEvent() {
	GenEnnemi* gen;

	if (tmp)
		gen = new GenEnnemiTMP();
	else
		gen = new GenEnnemi();

	gen->id_ennemi = id_ennemi;
	gen->x = x;
	gen->y = y;
	gen->sens = sens;
	gen->periode = periode;
	gen->capacite = capacite;

	list_gen_ennemis.emplace_back(gen);
}
