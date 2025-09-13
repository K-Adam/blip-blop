#pragma once

#include "entities/enemies/enemy.h"

class EnemySpikes : public Enemy {
   private:
	int y_origine;

   public:
	EnemySpikes();
	void update() override;
	void render() override;
	void colFromPic() override;
	void takeHit(Shot* shot) override;
	int damage() override { return 5; };

	bool count() override { return false; };
};
