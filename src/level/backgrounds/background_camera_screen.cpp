#include "core/globals.h"
#include "level/backgrounds/background_camera_screen.h"

void FondEcrancamera::affiche() {
	Sprite::affiche();
	draw(x, y + ss_etape, pbk_niveau[49]);
}
