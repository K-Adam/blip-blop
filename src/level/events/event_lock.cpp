#include "level/events/event_lock.h"
#include "level/world.h"
#include "core/debug.h"

void EventLock::doEvent() {
	level().camera.lock.on = true;
	level().camera.lock.x = x_activation;
	level().camera.lock.cond = cond;
	level().camera.lock.flag = flag;
	level().camera.lock.val = val;
}
