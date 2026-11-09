#include "level/events/event_music.h"
#include "core/assets.h"

void EventMusic::doEvent() {
	if (play) {
		mbk_level.stop();

		mbk_level.play(id);
		current_track = id;
	} else {
		mbk_level.stop();
		current_track = -1;
	}
}
