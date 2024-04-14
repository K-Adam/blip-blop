#include "level/events/event_foreground.h"
#include "entities/sprite.h"

class PPlan : public Sprite {
   public:
	virtual void update(){};
};

void EventPremierPlan::doEvent() {
	PPlan* s = new PPlan();

	s->x = x;
	s->y = y;
	s->pic = pbk_niveau[id_fond];

	list_premiers_plans.emplace_back(s);
}
