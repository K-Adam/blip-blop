#include "level/events/event_bonus.h"
#include "entities/bonus_weapon.h"
#include "entities/bonus_beer.h"
#include "entities/bonus_invincible.h"
#include "entities/bonus_barrel.h"
#include "entities/bonus_cow.h"

void EventBonus::doEvent() {
	Bonus* bonus;

	switch (type) {
		case BPM:
			bonus = new BonusPM();
			break;

		case BFUSIL:
			bonus = new BonusFusil();
			break;

		case BLF:
			bonus = new BonusLF();
			break;

		case BLASER:
			bonus = new BonusLaser();
			break;

		case BBEER:
			bonus = new BonusBeer();
			break;

		case BINV:
			bonus = new BonusInvincible();
			break;

		case BTONNO:
			bonus = new BonusTonneau();
			break;

		case BCOW:
			bonus = new BonusVache();
			break;

		default:
			bonus = NULL;
			debug << "Unknown bonus type\n";
			break;
	}

	if (bonus != NULL) {
		bonus->x = x;
		bonus->y = y;

		list_bonus.emplace_back(bonus);
	}
}
