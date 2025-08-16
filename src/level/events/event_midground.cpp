#include "level/world.h"
#include "core/assets.h"
#include "level/events/event_midground.h"
#include "level/backgrounds/background_static.h"

void EventMidground::doEvent() {
	Sprite* s = new BackgroundStatic();

	s->x = x;
	s->y = y;
	s->pic = pbk_niveau[id];

	list_fonds_statiques.emplace_back(s);
}
