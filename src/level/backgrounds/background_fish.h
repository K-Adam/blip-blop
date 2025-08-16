#pragma once

#define POISSON_SPEED 1

class BackgroundFishRed : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishRed();
	void update() override;
};

class BackgroundFishPurple : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishPurple();
	void update() override;
};

class BackgroundFishRedCamouflaged : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishRedCamouflaged();
	void update() override;
};

class BackgroundFishPurpleCamouflaged : public Sprite {
   private:
	int dir;

   public:
	BackgroundFishPurpleCamouflaged();
	void update() override;
};
