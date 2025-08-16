#include "level/events/event_bonus.h"

#define BONUS_SEED 20

int makeb_current_mode = 0;

int bonus_list[][BONUS_SEED] = {

	// Mode 0 suits bonus levels
	{BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL},

	{BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL, BPM, BFUSIL},

	{BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF},

	{BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER,
	 BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER},

	{BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM},

	{BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL,
	 BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL, BFUSIL},

	{BPM, BFUSIL, BPM, BFUSIL, BLF, BPM, BFUSIL, BFUSIL, BFUSIL, BLF, BPM, BFUSIL, BFUSIL, BFUSIL, BLF, BPM, BFUSIL, BPM, BFUSIL, BLF},

	{BPM, BFUSIL, BPM, BFUSIL, BLASER, BPM, BFUSIL, BFUSIL, BFUSIL, BLASER, BPM, BFUSIL, BFUSIL, BFUSIL, BLASER, BPM, BFUSIL, BPM, BFUSIL, BLASER},

	{BPM, BFUSIL, BPM, BPM, BFUSIL, BBEER, BPM, BFUSIL, BFUSIL, BPM, BFUSIL, BPM, BTONNO, BPM, BFUSIL, BPM, BFUSIL, BINV, BFUSIL, BPM},
};

void MakeBonus(int x, int y) {
	static int seed = 0;

	seed += 1;
	seed %= BONUS_SEED;

	int n = bonus_list[makeb_current_mode][seed];

	EventBonus eb;

	eb.x = x;
	eb.y = y;
	eb.type = n;
	eb.doEvent();
}

void MakeBonusWeapon(int x, int y) {
	static int seed = 0;

	seed += 1;
	seed %= BONUS_SEED;

	int n = bonus_list[1][seed];

	EventBonus eb;

	eb.x = x;
	eb.y = y;
	eb.type = n;
	eb.doEvent();
}
