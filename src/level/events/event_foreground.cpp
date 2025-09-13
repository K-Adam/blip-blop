#include "level/events/event_foreground.h"
#include "entities/sprite.h"

class PPlan : public Sprite {
   public:
	void update() override{};
};

void EventForeground::doEvent() {
	PPlan* s = new PPlan();

	s->x = x;
	s->y = y;
	s->pic = pbk_level[bg_id];

	list_foregrounds.emplace_back(s);
}
