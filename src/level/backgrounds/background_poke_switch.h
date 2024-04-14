#pragma once

class BackgroundPokeSwitch : public Sprite {
   private:
	bool etat;
	bool wait_msg;
	bool ascenc_msg;
	bool bravo;

   public:
	BackgroundPokeSwitch();
	virtual void update();
};
