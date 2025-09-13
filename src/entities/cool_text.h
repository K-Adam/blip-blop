#pragma once

#ifndef COOL_TEXT_CPP
extern bool txt_cool_free[];
#endif

class CoolText {
   public:
	int y;
	int nn;
	int ntxt;
	int x;
	int t;

	CoolText();

	void update();
	void render();

	bool isDestroyed() { return x <= -320; };
};

void clearCoolText();
