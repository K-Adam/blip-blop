#include "level/events/event_music.h"
#include "core/assets.h"

void EventMusic::doEvent() {
	if (play) {
		if (current_track != -1) mbk_level.stop(current_track);

		mbk_level.play(id);
		current_track = id;
	} else {
		mbk_level.stop(id);
		current_track = -1;
	}
}
