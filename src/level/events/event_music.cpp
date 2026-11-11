#include "level/events/event_music.h"
#include "core/assets.h"

void EventMusic::doEvent() {
	assets().music.level.stop();
	if (play) assets().music.level.play(id);
}
