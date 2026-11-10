#include "entities/enemies/enemy_spikes.h"

EnemySpikes::EnemySpikes() : y_origin(0) {
	hp = 1;
	pic = assets().pics.enemies.pic("spikes");
}

void EnemySpikes::update() {
	if ((level().rules.flags[0] == 3) || (level().rules.flags[0] == 2)) {
		if (-104 > y_origin) {
			// Fully out, nothing left to raise
		} else if (-96 > y_origin) {
			y -= 1;
			y_origin -= 1;
		} else if (-86 > y_origin) {
			y -= 2;
			y_origin -= 2;
		} else if (-66 > y_origin) {
			y -= 4;
			y_origin -= 4;
		} else if (-46 > y_origin) {
			y -= 6;
			y_origin -= 6;
		} else if (-26 > y_origin) {
			y -= 8;
			y_origin -= 8;
		} else if (0 >= y_origin) {
			y -= 10;
			y_origin -= 10;
		}
		colFromPic();
	} else if (level().rules.flags[0] >= 5) {
		if (-104 > y_origin) {
			y += 10;
			y_origin += 10;
		} else if (-96 > y_origin) {
			y += 8;
			y_origin += 8;
		} else if (-86 > y_origin) {
			y += 6;
			y_origin += 6;
		} else if (-66 > y_origin) {
			y += 4;
			y_origin += 4;
		} else if (-46 > y_origin) {
			y += 3;
			y_origin += 3;
		} else if (-26 > y_origin) {
			y += 2;
			y_origin += 2;
		} else if (0 >= y_origin) {
			y += 1;
			y_origin += 1;
		} else {
			// Fully retracted, safe to remove
			for (int i = 28; i <= 55; i++) level().terrain.solid[i][108] = false;

			destroyed = true;
		}
		colFromPic();
	}
}

void EnemySpikes::render() {
	Rect r;

	r.top = 0;
	r.left = 0;

	r.right = 39;
	r.bottom = -y_origin;

	assets().pics.enemies.pic("spikes")->BlitPart(graphics.back(), x - level().camera.offset, y, r);
}

void EnemySpikes::colFromPic() {
	Sprite::colFromPic();

	x2 += 25;
	x1 += 25;
}

void EnemySpikes::takeHit(Shot* shot) {}
