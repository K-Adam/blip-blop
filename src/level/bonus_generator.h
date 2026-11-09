#pragma once

class BonusGenerator {
   public:
	virtual ~BonusGenerator() = default;
	int x;
	int period;
	int t;
	int type;
	bool destroyed;

	BonusGenerator();

	virtual void update();

	inline virtual bool isDestroyed() { return destroyed; };
};
