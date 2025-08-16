#include "level/events/event_lock.h"
#include "level/world.h"
#include "core/debug.h"

void EventLock::doEvent() {
	scroll_locked = true;
	x_lock = x_activation;
	cond_end_lock = cond;
	flag_end_lock = flag;
	val_end_lock = val;
}
