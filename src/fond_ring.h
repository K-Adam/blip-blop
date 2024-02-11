#pragma once

class FondRing : public Sprite {
   private:
	static int etape_synchro;
	static int ss_etape_synchro;
	static bool synchro;

   public:
	virtual void update();
};
