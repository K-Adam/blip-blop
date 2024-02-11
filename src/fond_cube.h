#pragma once

class FondCube : public Sprite {
   private:
	static int etape_synchro;
	static bool synchro;
	static int ss_etape_synchro;

   public:
	virtual void update();
};
