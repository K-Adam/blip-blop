#pragma once

#include "entities/enemies/enemy.h"

class EnemySpikes : public Enemy {
   private:
	int y_origine;

   public:
	EnemySpikes();
	void update() override;
	void affiche() override;
	void colFromPic() override;
	void estTouche(Shot* tir) override;
	int degats() override { return 5; };

	bool count() override { return false; };
};
