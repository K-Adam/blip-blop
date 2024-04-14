#pragma once

#define POISSON_SPEED 1

class BackgroundFishRed : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishRed();
	virtual void update();
};

class BackgroundFishPurple : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishPurple();
	virtual void update();
};

class BackgroundFishRedCamouflaged : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishRedCamouflaged();
	virtual void update();
};

class BackgroundFishPurpleCamouflaged : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishPurpleCamouflaged();
	virtual void update();
};
