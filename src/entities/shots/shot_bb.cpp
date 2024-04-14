#include "entities/shots/shot_bb.h"

void TirBB::setDir(int d) {
	// Convertit la dir BB en dir TIR
	if (d == BBDIR_BAS_B_G)
		dir = 0;
	else if (d > BBLIM_DROITE)
		dir = d - 1;
	else
		dir = d;
}
