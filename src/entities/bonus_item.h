#pragma once

#include <algorithm>

#include "entities/bonus.h"

struct ItemSpec {
	const char* pic;
	int sound;			   // -1 for none
	bool (*apply)(Ball*);  // False leaves the item lying, for a full player
};

class BonusItem : public Bonus {
   public:
	explicit BonusItem(const ItemSpec& s) : s_(s) {
		pic = assets().pics.misc.pic(s.pic);
		col_on = true;
	}

	void onPickup(Ball* c) override {
		if (!s_.apply(c)) return;
		if (s_.sound >= 0) assets().sounds.misc.play(s_.sound);
		Bonus::onPickup(c);
	}

   private:
	ItemSpec s_;
};

class BonusBarrel : public BonusItem {
   public:
	BonusBarrel()
		: BonusItem({"bonus_barrel", 5, [](Ball* c) {
						 c->hp = 5;
						 return true;
					 }}) {}
};

class BonusCow : public BonusItem {
   public:
	BonusCow()
		: BonusItem({"bonus_cow", 1, [](Ball* c) {
						 c->nb_cow_bomb += 1;
						 return true;
					 }}) {}
};

class BonusBeer : public BonusItem {
   public:
	BonusBeer()
		: BonusItem({"bonus_beer", 5, [](Ball* c) {
						 if (c->hp == 5) return false;
						 c->hp = std::min(c->hp + 2, 5);
						 return true;
					 }}) {}
};

class BonusInvincible : public BonusItem {
   public:
	BonusInvincible()
		: BonusItem({"bonus_invincible", -1, [](Ball* c) {
						 c->invincible = 550;
						 return true;
					 }}) {}
};
