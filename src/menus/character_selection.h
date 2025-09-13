#pragma once

#define APPEAR_BLIP 0
#define DONE_BLIP 1
#define APPEAR_BLOP 2
#define DONE_BLOP 3

class CharacterSelection {
	int phase_tick_;
	int phase_;
	int x_back1_;
	int x_back2_;
	int y_select_;
	int x_name_;
	int y_name_;
	int step_;
	int pic_select_;
	int x_charac_;
	int y_charac_;

   public:
	enum class Output : int {
		Blip = 0,
		Blop = 1,
		Continue = 2,
	};
	CharacterSelection();
	Output update();
	void draw();
};
