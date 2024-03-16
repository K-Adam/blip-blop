#include "globals.h"
#include "fond_ecrancamera.h"

void FondEcrancamera::affiche() {
	Sprite::affiche();
	draw(x, y + ss_etape, pbk_niveau[49]);
}
