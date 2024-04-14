#pragma once

class GenBonus {
   public:
	virtual ~GenBonus() = default;
	int x;
	int periode;
	int t;
	int type;
	bool a_detruire;

	GenBonus();

	virtual void update();

	inline virtual bool aDetruire() { return a_detruire; };
};
