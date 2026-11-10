#include "level/events/event_foreground.h"
#include "entities/sprite.h"

class ForegroundLayer : public Sprite {
   public:
	void update() override{};
};

void EventForeground::doEvent() {
	ForegroundLayer* s = new ForegroundLayer();

	s->x = x;
	s->y = y;
	s->pic = assets().pics.level.frame(pic);

	level().entities.foregrounds.emplace_back(s);
}
