#include "level/events/event_bonus.h"
#include "entities/bonus_weapon.h"
#include "entities/bonus_item.h"

void EventBonus::doEvent() {
	Bonus* bonus;

	switch (type) {
		case BPM:
			bonus = new BonusPM();
			break;

		case BSHOTGUN:
			bonus = new BonusShotgun();
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

		case BBARREL:
			bonus = new BonusBarrel();
			break;

		case BCOW:
			bonus = new BonusCow();
			break;

		default:
			bonus = nullptr;
			debug << "Unknown bonus type\n";
			break;
	}

	if (bonus != nullptr) {
		bonus->x = x;
		bonus->y = y;

		level().entities.bonus.emplace_back(bonus);
	}
}
