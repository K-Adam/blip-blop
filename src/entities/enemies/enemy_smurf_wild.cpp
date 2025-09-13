const int tir_position_depart_x[] = {-15, -8, 6, 1, 8, 17};
const int tir_position_depart_y[] = {55, 62, 65, 65, 62, 55};

const int tir_dx[] = {-4, -1, 0, 0, 1, 4};
const int tir_dy[] = {4, 5, 6, 6, 5, 4};

#include "entities/enemies/enemy_smurf_wild.h"
#include "entities/shots/shot_smurf_wild.h"

int EnemySmurfWild::wait_shoot;
int EnemySmurfWild::etape_wait;

EnemySmurfWild::EnemySmurfWild() : etape_shoot(0), position(false) {
	first_loop = true;
	hp = 150;
	anim_step = 1;
	nb_shoot = 0;
	mort = false;
	cible = targeted_player;
	ss_etape_angle = angle = 0;
	sensAngle = true;

	wait_shoot = 100 + rand() % 100;
	etape_wait = 0;
	status = ETAT_ATTENDS;

	switch (dir) {
		case FACING_RIGHT:
			num_pic = 139;
			pic = pbk_enemies[140];
			shoot_direction = BAS_DROITE;
			break;
		case FACING_LEFT:
			num_pic = 141;
			pic = pbk_enemies[142];
			shoot_direction = BAS_GAUCHE;
			break;
	}
}

void EnemySmurfWild::update() {
	if (x > 5850) x = 5850;

	shooting = false;

	if (first_loop) {
		y = -100;
		first_loop = false;
	}

	if (cible == NULL || cible->isDestroyed()) {
		cible = targeted_player;
	}

	if (status != ETAT_MEURE) {
		if (cible != NULL && x != cible->x)
			pente = (abs(cible->y - y) * 10) / (x - cible->x);
		else
			pente = 3;
	}

	if (pente > 0) {
		dir = FACING_LEFT;
		num_pic = 141;

		if (pente <= 15)
			shoot_direction = DIAGONALE_GAUCHE;
		else if (pente <= 25)
			shoot_direction = DIAGONALE_BAS_GAUCHE;
		else
			shoot_direction = BAS_GAUCHE;
	} else if (pente < 0) {
		dir = FACING_RIGHT;
		num_pic = 139;

		if (pente >= -15)
			shoot_direction = DIAGONALE_DROITE;
		else if (pente >= -25)
			shoot_direction = DIAGONALE_BAS_DROITE;
		else
			shoot_direction = BAS_DROITE;
	}

	if (status != ETAT_MEURE) pic = pbk_enemies[num_pic + anim_step];

	if (blood != 0) blood -= 1;

	if (!mort && hp < 0) {
		mort = true;
		anim_step = 0;
		status = 0;
	}

	switch (status) {
		case ETAT_ATTENDS:
			if (++etape_wait >= wait_shoot) {
				status = ETAT_TOMBE;

				sbk_level.play(17);

				if (rand() % 2 == 0)
					x = offset + 320 - rand() % 150;
				else
					x = offset + 320 + rand() % 150;

				int j = 100 + rand() % 140;

				while (solid_wall(x, j)) j = 100 + rand() % 140;

				nb_shoot = 0;
				hauteur = j;
			}
			break;
		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_MONTE:
			etape_wait = 0;

			if (y <= -100) {
				wait_shoot = 100 + rand() % 100;
				status = ETAT_ATTENDS;
				destroyed = true;
				cible = NULL;
			} else {
				y -= 2;
			}
			colFromPic();

			break;

		case ETAT_MEURE:
			onMeure();
			break;
		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}
	updateDestroyed();
}

void EnemySmurfWild::onTombe() {
	if (y < hauteur) {
		y += 3;
	} else {
		status = ETAT_TIRE;
	}

	etape_wait = 0;
	colFromPic();
}

void EnemySmurfWild::onTire() {
	shooting = true;

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 2) anim_step = 1;

	etape_shoot += 1;
	etape_shoot %= 10;

	if (etape_shoot == 0) {
		ShotSmurfWild* shot = new ShotSmurfWild();
		shot->x = x + tir_position_depart_x[shoot_direction];
		shot->y = y + tir_position_depart_y[shoot_direction];
		shot->dx = tir_dx[shoot_direction];
		shot->dy = tir_dy[shoot_direction];
		list_enemy_shots.emplace_back(shot);

		if (nb_shoot == 0) sbk_level.play(18);
		nb_shoot += 1;
	}

	if (nb_shoot >= 5) status = ETAT_MONTE;

	colFromPic();
}

void EnemySmurfWild::takeHit(Shot* shot) {
	static const int dx_giclure_smurf_sauvage[] = {0, 0, 6, 0, 0, 0, -6, 0};
	static const int dy_giclure_smurf_sauvage[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	if (status == ETAT_MEURE) sbk_level.play(4 + rand() % 10);
}

void EnemySmurfWild::onMeure() {
	if (anim_step >= 5) {
		bool ok = true;

		colFromPic();
		y1 = 0;

		for (auto& s : list_static_bgs) {
			ok = ok && !collision(s.get());
		}

		col_on = false;

		if (ok) {
			engrave(x, y - 50, pbk_enemies[117]);
			engrave(x, y - 100, pbk_enemies[117]);
			engrave(x, y - 150, pbk_enemies[117]);
			engrave(x, y - 200, pbk_enemies[117]);
			engrave(x, y - 250, pbk_enemies[117]);
			engrave(x, y, pic);
			destroyed = true;
		}
	} else {
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) {
			anim_step += 1;
			if (dir == FACING_RIGHT)
				pic = pbk_enemies[117 + anim_step];
			else
				pic = pbk_enemies[122 + anim_step];
		}
	}
}

void EnemySmurfWild::onCarbonise() {
	y = hauteur + 50 - anim_step;
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}
