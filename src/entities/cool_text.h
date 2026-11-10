#pragma once

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
