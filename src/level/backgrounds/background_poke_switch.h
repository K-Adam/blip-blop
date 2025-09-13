#pragma once

class BackgroundPokeSwitch : public Sprite {
   private:
	bool status;
	bool wait_msg;
	bool elevator_msg;
	bool bravo;

   public:
	BackgroundPokeSwitch();
	void update() override;
};
