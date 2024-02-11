#pragma once

class FondPokeInterupteur : public Sprite {
   private:
	bool etat;
	bool wait_msg;
	bool ascenc_msg;
	bool bravo;

   public:
	FondPokeInterupteur();
	virtual void update();
};
