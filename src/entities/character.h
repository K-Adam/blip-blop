#pragma once

#include "entities/sprite.h"

#define STATE_WALK 0
#define STATE_JUMP 1
#define STATE_FALL 2
#define STATE_SHOOT 3
#define STATE_DYING 4
#define STATE_WAIT 5
#define STATE_NORMAL 6
#define STATE_CHARRED 7
#define STATE_BURN 8
#define STATE_COME_BACK 9
#define STATE_SOMERSAULT 10
#define STATE_DYING_2 11
#define STATE_DYING_3 12
#define STATE_DYING_4 13
#define STATE_LOCKED_VEHICLE 14

#ifndef FACING
#define FACING_LEFT 0
#define FACING_RIGHT 1
#endif

class Character : public Sprite {
   public:
	int hp;
	int status;

	Character();

	void update() override;
};
