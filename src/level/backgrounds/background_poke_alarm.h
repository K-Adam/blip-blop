#pragma once

#include "entities/sprite.h"

class BackgroundPokeAlarm1 : public Sprite {
   public:
	virtual void update() {
		ss_etape += 1;
		ss_etape %= 3;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 14;
		}

		pic = pbk_niveau[etape];
		updateADetruire();
	};
};

class BackgroundPokeAlarm2 : public Sprite {
   public:
	virtual void update() {
		ss_etape += 1;
		ss_etape %= 3;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 14;
		}

		pic = pbk_niveau[14 + etape];
		updateADetruire();
	};
};

class BackgroundPokeAlarm3 : public Sprite {
   public:
	virtual void update() {
		ss_etape += 1;
		ss_etape %= 3;

		if (ss_etape == 0) {
			etape += 1;
			etape %= 14;
		}

		pic = pbk_niveau[28 + etape];
		updateADetruire();
	};
};
