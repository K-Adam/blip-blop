#pragma once

#include "entities/sprite.h"

#define ETAT_AVANCE 0
#define ETAT_SAUTE 1
#define ETAT_TOMBE 2
#define ETAT_TIRE 3
#define ETAT_MEURE 4
#define ETAT_ATTEND 5
#define STATE_NORMAL 6
#define ETAT_CARBONISE 7
#define ETAT_BRULE 8
#define STATE_COME_BACK 9
#define ETAT_SALETO 10
#define ETAT_MEURE_2 11
#define ETAT_MEURE_3 12
#define ETAT_MEURE_4 13
#define ETAT_LOCKEDV 14

#ifndef SENS
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
