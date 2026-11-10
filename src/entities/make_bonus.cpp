#include "level/events/event_bonus.h"

#define BONUS_SEED 20

int bonus_list[][BONUS_SEED] = {

	// Mode 0 suits bonus levels
	{BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN},

	{BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN, BPM, BSHOTGUN},

	{BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF, BLF},

	{BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER,
	 BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER, BLASER},

	{BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM, BPM},

	{BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN,
	 BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN, BSHOTGUN},

	{BPM, BSHOTGUN, BPM, BSHOTGUN, BLF, BPM, BSHOTGUN, BSHOTGUN, BSHOTGUN, BLF, BPM, BSHOTGUN, BSHOTGUN, BSHOTGUN, BLF, BPM, BSHOTGUN, BPM, BSHOTGUN, BLF},

	{BPM, BSHOTGUN, BPM,	  BSHOTGUN, BLASER, BPM, BSHOTGUN, BSHOTGUN, BSHOTGUN, BLASER,
	 BPM, BSHOTGUN, BSHOTGUN, BSHOTGUN, BLASER, BPM, BSHOTGUN, BPM,		 BSHOTGUN, BLASER},

	{BPM, BSHOTGUN, BPM, BPM, BSHOTGUN, BBEER, BPM, BSHOTGUN, BSHOTGUN, BPM, BSHOTGUN, BPM, BBARREL, BPM, BSHOTGUN, BPM, BSHOTGUN, BINV, BSHOTGUN, BPM},
};

namespace {
// Walks the bonus table in order so drops vary
struct BonusSeeds {
	int any = 0;
	int weapon = 0;
};
}  // namespace

void MakeBonus(int x, int y) {
	int& seed = session().shared<BonusSeeds>().any;

	seed += 1;
	seed %= BONUS_SEED;

	int n = bonus_list[level().rules.bonus_mode][seed];

	EventBonus eb;

	eb.x = x;
	eb.y = y;
	eb.type = n;
	eb.doEvent();
}

void MakeBonusWeapon(int x, int y) {
	int& seed = session().shared<BonusSeeds>().weapon;

	seed += 1;
	seed %= BONUS_SEED;

	int n = bonus_list[1][seed];

	EventBonus eb;

	eb.x = x;
	eb.y = y;
	eb.type = n;
	eb.doEvent();
}
