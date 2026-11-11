#pragma once

#include "entities/sprite.h"

// Shared by all characters. Entities add their own states as constexpr int, so status stays an open set
enum {
	STATE_WALK,
	STATE_JUMP,
	STATE_FALL,
	STATE_SHOOT,
	STATE_DYING,
	STATE_WAIT,
	STATE_NORMAL,
	STATE_CHARRED,
	STATE_BURN,
	STATE_COME_BACK,
	STATE_SOMERSAULT,
	STATE_DYING_2,
	STATE_DYING_3,
	STATE_DYING_4,
	STATE_LOCKED_VEHICLE,
};

class Character : public Sprite {
   public:
	int hp;
	int status;

	Character();

	void update() override;
};
