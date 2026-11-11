#include "level/world.h"
#include "core/assets.h"
#include "level/events/event_midground.h"
#include "level/backgrounds/background_static.h"

void EventMidground::doEvent() {
	auto s = std::make_unique<BackgroundStatic>();

	s->x = x;
	s->y = y;
	s->pic = assets().pics.level.frame(pic);

	level().entities.static_bgs.emplace_back(std::move(s));
}
