#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_camera_screen.h"

void BackgroundCameraScreen::affiche() {
	Sprite::affiche();
	draw(x, y + ss_etape, pbk_niveau[49]);
}
