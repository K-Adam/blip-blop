#include "level/events/event_bonus.h"
#include "entities/bonus_weapon.h"
#include "entities/bonus_item.h"

void EventBonus::doEvent() {
	std::unique_ptr<Bonus> bonus;

	switch (type) {
		case BPM:
			bonus = std::make_unique<BonusPM>();
			break;

		case BSHOTGUN:
			bonus = std::make_unique<BonusShotgun>();
			break;

		case BLF:
			bonus = std::make_unique<BonusLF>();
			break;

		case BLASER:
			bonus = std::make_unique<BonusLaser>();
			break;

		case BBEER:
			bonus = std::make_unique<BonusBeer>();
			break;

		case BINV:
			bonus = std::make_unique<BonusInvincible>();
			break;

		case BBARREL:
			bonus = std::make_unique<BonusBarrel>();
			break;

		case BCOW:
			bonus = std::make_unique<BonusCow>();
			break;

		default:
			bonus = nullptr;
			debug << "Unknown bonus type\n";
			break;
	}

	if (bonus != nullptr) {
		bonus->x = x;
		bonus->y = y;

		level().entities.bonus.emplace_back(std::move(bonus));
	}
}
