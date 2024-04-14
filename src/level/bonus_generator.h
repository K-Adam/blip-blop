#pragma once

class BonusGenerator {
   public:
	virtual ~BonusGenerator() = default;
	int x;
	int periode;
	int t;
	int type;
	bool a_detruire;

	BonusGenerator();

	virtual void update();

	inline virtual bool aDetruire() { return a_detruire; };
};
