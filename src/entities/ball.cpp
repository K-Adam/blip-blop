#include "entities/ball.h"
#include "entities/shots/shot_bb_cow.h"

#include <algorithm>
#include <cmath>
#include <numbers>

const int anim_walk[] = {0, 1, 2, 1};
const int anim_jump[] = {2, 2, 2, 1, 1, 0, 0, 3, 3, 3, 3};

const int hop_offsets[] = {0, 2, 4, 2};

Ball::Ball(const PictureBank& pbk)
	: pbk_own(pbk),
	  perfect(true),
	  ctrl(nullptr),
	  hop_idx(0),
	  weapon_id(ID_M16),
	  weapon_dir(0),
	  ammo{},
	  weapon_step(0),
	  weapon_tick(0),
	  weapon_anim_delay(3),
	  gun_fire_tick(0),
	  gun_turn_tick(0),
	  is_firing(false),
	  dx_jump(0),
	  dx_slide(0),
	  slide_delay(0),
	  recoil_step(0),
	  invincible(0),
	  inv_cow(false),
	  hurt_timer(0),
	  blink_step(0),
	  y_to_go(0),
	  nb_life(5),
	  nb_cow_bomb(0),
	  wait_cow_bomb(0),
	  jump_released(false),
	  time_down(0),
	  locked_fire(false),
	  locked_dir(false),
	  mod_life(0) {
	dir = BBDIR_RIGHT;
	col_on = true;
	hp = 5;
	equip(ID_M16);
}

void Ball::render() {
	if (nb_life <= 0) return;

	if (invincible > 50 && blink_step == 0 && session().phase && !inv_cow) return;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			renderNormal();
			break;

		case STATE_SOMERSAULT:
		case STATE_COME_BACK:
			Sprite::render();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			renderJump();
			break;

		case STATE_DYING:
			renderDying();
			break;

		case STATE_LOCKED_VEHICLE:
			renderVehicle();
			break;
	}
}

void Ball::renderNormal() {
	int yback = y;
	y -= hop_offsets[hop_idx];

	Sprite::render();

	renderEye(x, y);
	renderWeapon(x, y);

	y = yback;
}

// Counterclockwise degrees, the right side spans -90..90 and the left side 90..270
static double angleOf(int d) { return d <= BBLIM_RIGHT ? (d - BBDIR_RIGHT) * 22.5 : 180 + (d - BBDIR_LEFT) * 22.5; }

double Ball::gunAngle() const {
	double a = angleOf(weapon_dir);
	// The exact aim goes stale while a vehicle locks the direction
	bool locked = status == STATE_LOCKED_VEHICLE && locked_dir;
	bool precise = !locked && weapon_dir == dir && std::abs(aim_angle_ - a) <= 11.25;
	return precise ? aim_angle_ : a;
}

GunPose Ball::gunPose() const {
	// Hand drawn sprites cover 45 degree steps, the rest of the aim rotates them around the grip
	double a = gunAngle();
	bool right = weapon_dir <= BBLIM_RIGHT;
	// Sprites 0..4 run from down to up on the right, 5..9 from up to down on the left
	int step = std::clamp(static_cast<int>(std::lround(right ? (a + 90) / 45 : (a - 90) / 45)), 0, 4);
	double drawn = right ? -90 + 45 * step : 90 + 45 * step;
	return {right ? step : 5 + step, a - drawn, a};
}

void Ball::renderWeapon(int xtmp, int ytmp) { weapon().draw(*this, xtmp, ytmp, gunPose()); }

void Ball::renderEye(int xtmp, int ytmp) {
	int delta;

	if (hurt_timer) {
		if (dir <= BBLIM_RIGHT)
			draw(xtmp + 12, ytmp - 14, pbk_own.pic("eye_hurt_right"));
		else
			draw(xtmp - 12, ytmp - 14, pbk_own.pic("eye_hurt_left"));
	} else {
		Flipbook eyes = assets().pics.bb.anim(is_firing ? "eye_angry" : "eye_normal");

		if (dir <= BBDIR_DOWN_RIGHT)
			delta = 0;
		else if (dir <= BBDIR_RIGHT)
			delta = 1;
		else if (dir <= BBDIR_UP_U_R)
			delta = 2;
		else if (dir <= BBDIR_UP_LEFT)
			delta = 3;
		else if (dir <= BBDIR_LEFT)
			delta = 4;
		else
			delta = 5;

		if (dir <= BBLIM_RIGHT)
			draw(xtmp + 12, ytmp - 14, eyes[delta]);
		else
			draw(xtmp - 12, ytmp - 14, eyes[delta]);
	}
}

void Ball::renderJump() {
	Sprite::render();

	if (dy < 0) {
		renderEye(x, y + dy - 2);
		renderWeapon(x, y + dy - 2);
	} else {
		renderEye(x, y);
		renderWeapon(x, y);
	}
}

void Ball::update() {
	if (session().phase) {
		blink_step += 1;
		blink_step %= 4;
	}

	int loop = is_firing ? weapon().stats.loop_sound : -1;
	if (loop != looping_sound_) {
		if (looping_sound_ >= 0) assets().sounds.bb.stop(looping_sound_);
		if (loop >= 0) assets().sounds.bb.play(loop, SOUND_LOOP);
		looping_sound_ = loop;
	}

	if (nb_life <= 0) return;

	if (y > 540) {
		nb_life = 0;
		perfect = false;
		destroyed = true;
	}

	if (status == STATE_DYING) {
		onDie();
		return;
	}

	if (status == STATE_COME_BACK) {
		onComeBack();
		return;
	}

	if (wait_cow_bomb < 100) wait_cow_bomb += 1;

	if (ctrl->super() && wait_cow_bomb == 100 && nb_cow_bomb >= 1) {
		ShotBBCow* ptr;

		for (int i = 0; i < 11; i++) {
			int xCow = level().camera.offset + i * 64 + rnd() % 20;

			if (!level().terrain.solid_wall(xCow, 0)) {
				ptr = new ShotBBCow();

				ptr->x = xCow;
				ptr->y = rnd() % 180 - 200;
				ptr->dy = 5 + rnd() % 3;
				ptr->anim_step = rnd() % 8;
				ptr->hero = this;

				level().entities.cows.push_back(std::unique_ptr<ShotBBCow>(ptr));
			}
		}

		wait_cow_bomb = 0;
		nb_cow_bomb -= 1;
		invincible = 200;
		inv_cow = true;

		assets().sounds.bb.play(5);
	}

	// The scroll pushes players along the left edge
	if (x < level().camera.offset + 20 && !ctrl->rightHeld() && !level().terrain.solid_wall(x + 2, y)) {
		x += 2;

		if (slide_delay < 0) dx_slide = slide_delay = 0;
	}

	int yp = level().terrain.platform_y(x, y);

	if (yp != 0) y = yp;

	// A player pinned against a wall by the scroll dies
	if (x < level().camera.offset - 30) {
		level().camera.frozen = true;
		nb_life -= 1;

		if (nb_life == 0) {
			destroyed = true;
			return;
		}

		status = STATE_COME_BACK;

		x = level().camera.offset + 320;
		int i = 0;

		while (i < NB_MAX_PLATFORMS && level().terrain.platforms[i][x] == 0xFFFF) i++;

		y_to_go = i < NB_MAX_PLATFORMS ? level().terrain.platforms[i][x] : 0xFFFF;

		y = -50;
		col_on = true;
		hp = 5;
		weapon_dir = dir = BBDIR_RIGHT;
		invincible = 400;
		dropWeapons();
		pic = pbk_own.anim("jump_right")[2];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_jump = 0;
	}

	if (invincible > 0) {
		invincible -= 1;
	} else {
		inv_cow = false;
	}

	if (hurt_timer > 0) hurt_timer -= 1;

	if (anim_tick == 0) {
		hop_idx++;
		hop_idx %= 4;
	}

	// Detect a quick double tap on down, which drops through the platform
	if (ctrl->downHeld()) {
		if (time_down > 0 && time_down < 10 && status == STATE_NORMAL)
			time_down = -1;
		else
			time_down = 0;
	} else {
		time_down += 1;
	}

	if (status != STATE_LOCKED_VEHICLE || !locked_dir) manageDirection();

	switch (status) {
		case STATE_NORMAL:
			onNormal();
			break;

		case STATE_WALK:
			onWalk();
			break;

		case STATE_SOMERSAULT:
			onSomersault();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			onJump();
			break;

		case STATE_LOCKED_VEHICLE:
			onVehicle();
			break;
	}

	// Ice keeps some momentum
	if (status == STATE_NORMAL || status == STATE_WALK) {
		int nplatform = level().terrain.platform_y2(x, y);

		if (nplatform >= 6 && x > 10 && x > level().camera.offset + 30 && x < level().camera.offset + 610) {
			int ytmp1 = level().terrain.platforms[nplatform][x + 10] - y;
			int ytmp2 = level().terrain.platforms[nplatform][x - 10] - y;

			if (ytmp1 < 10 && ytmp1 > -10) slide_delay += ytmp1;
			if (ytmp2 < 10 && ytmp2 > -10) slide_delay -= ytmp2;

			if (slide_delay > 0) {
				if (ctrl->rightHeld())
					slide_delay += 25;
				else if (ctrl->leftHeld())
					slide_delay -= 10;
				else
					slide_delay -= 1;
			} else if (slide_delay < 0) {
				if (ctrl->leftHeld())
					slide_delay -= 25;
				else if (ctrl->rightHeld())
					slide_delay += 10;
				else
					slide_delay += 1;
			} else {
				if (ctrl->rightHeld())
					slide_delay += 25;
				else if (ctrl->leftHeld())
					slide_delay -= 25;
			}

			if (slide_delay > 600)
				slide_delay = 600;
			else if (slide_delay < -600)
				slide_delay = -600;

			if (slide_delay > 0)
				dx_slide = slide_delay / 200 + 1;
			else if (slide_delay < 0)
				dx_slide = slide_delay / 200 - 1;
			else
				dx_slide = 0;
		} else {
			dx_slide = slide_delay = 0;
		}
	} else {
		dx_slide = slide_delay = 0;
	}

	colFromPic();

	if (status != STATE_SOMERSAULT) updateWeapon();

	return;
}

void Ball::onNormal() {
	if (time_down == -1 && !level().terrain.solid_wall(x, y + 5) && !level().terrain.solid_wall(x, y + 10) && !level().terrain.solid_wall(x, y + 15) &&
		!level().terrain.solid_wall(x, y + 20) && !level().terrain.solid_wall(x, y + 25)) {
		y += 10;
		dy = 5;
		status = STATE_FALL;
		grav_tick = 0;
		dx_jump = dx_slide;
		time_down = 0;
		onJump();
		return;
	}

	if (ctrl->jump() && !level().terrain.solid_wall(x, y - 5)) {
		status = STATE_JUMP;
		dy = -5;
		grav_tick = 0;	// Otherwise jump heights vary

		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		jump_released = false;
		onJump();
		return;
	}

	if (ctrl->leftHeld() || ctrl->rightHeld() || dx_slide != 0) {
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		return;
	}

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[animate(anim_walk, 4, 8)];
	else
		pic = pbk_own.anim("walk_left")[animate(anim_walk, 4, 8)];
}

void Ball::onWalk() {
	if (ctrl->jump() && !level().terrain.solid_wall(x, y - 5)) {
		status = STATE_JUMP;
		dy = -5;
		grav_tick = 0;	// Otherwise jump heights vary

		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		jump_released = false;
		onJump();
		return;
	}

	int destx = x + dx_slide;

	if ((dx_slide > 0 && !level().terrain.solid_wall(destx, y) && destx < level().camera.offset + 620) ||
		(dx_slide < 0 && !level().terrain.solid_wall(destx, y) && destx > level().camera.offset + 20))
		walk(dx_slide);
	else {
		if (ctrl->rightHeld() && (!level().terrain.solid_wall(x + 2, y) && x + 2 < level().camera.offset + 620)) {
			walk(2);
		} else if (ctrl->leftHeld() && (!level().terrain.solid_wall(x - 2, y) && x - 2 > level().camera.offset + 20)) {
			walk(-2);
		} else {
			status = STATE_NORMAL;
		}
	}

	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		dx_jump = dx_slide;

		if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;

		onJump();
		return;
	}

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[animate(anim_walk, 4, 5)];
	else
		pic = pbk_own.anim("walk_left")[animate(anim_walk, 4, 5)];
}

void Ball::onJump() {
	int yp;

	// A somersault needs jump released and pressed again mid jump
	if (!ctrl->jump()) jump_released = true;

	if (ctrl->jump() && jump_released && status == STATE_JUMP && dy < 3) {
		dy = -5;
		status = STATE_SOMERSAULT;
		anim_step = 0;
		anim_tick = 0;
		is_firing = false;
		weapon_step = weapon_tick = 0;
		onSomersault();
		return;
	}

	fall2();

	// Releasing jump early makes a shorter jump
	if (dy < 0 && !ctrl->jump()) {
		grav_tick += 1;
		grav_tick %= GRAVITY_DELAY;

		if (grav_tick == 0) {
			dy += 1;
		}
	}

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		slide_delay = 200 * dx_jump;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (!ctrl->inertia() || (!ctrl->downHeld() && !ctrl->upHeld())) {
		if (ctrl->rightHeld() && dx_jump < 2)
			dx_jump = 2;
		else if (ctrl->leftHeld() && dx_jump > -2)
			dx_jump = -2;
		else if (!ctrl->inertia())
			dx_jump = 0;
	}

	int xx = x + dx_jump;

	if (!level().terrain.solid_wall(xx, y) && xx > level().camera.offset + 20 && xx < level().camera.offset + 620) x = xx;

	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("jump_right")[anim_jump[dy + 5]];
	else
		pic = pbk_own.anim("jump_left")[anim_jump[dy + 5]];
}

void Ball::manageDirection() {
	// The pivot is the gun height, the feet are at y. No aim returns to horizontal
	int vx, vy;
	ctrl->aim(x - level().camera.offset, y - 12, vx, vy);
	double t = std::atan2(-vy, std::abs(vx));
	int s = static_cast<int>(std::lround(t / (std::numbers::pi / 8)));
	bool right = dir <= BBLIM_RIGHT;
	// Straight up or down keeps the facing, so the body does not flip while aiming there
	if (std::abs(s) < 4 && vx != 0) right = vx > 0;
	dir = right ? BBDIR_RIGHT + s : BBDIR_LEFT - s;
	double deg = t * 180 / std::numbers::pi;
	// Mirrored by the aim itself, then kept in the facing's range so near vertical aim stays on its side
	aim_angle_ = vx < 0 ? 180 - deg : deg;
	if (right && aim_angle_ > 180) aim_angle_ -= 360;
	if (!right && aim_angle_ < 0) aim_angle_ += 360;

	if (dir <= BBLIM_RIGHT && weapon_dir > BBLIM_RIGHT)
		weapon_dir = BBDIR_DOWN_D_L - weapon_dir;
	else if (dir > BBLIM_RIGHT && weapon_dir <= BBLIM_RIGHT)
		weapon_dir = 17 - weapon_dir;

	gun_turn_tick += 1;
	gun_turn_tick %= weapon().stats.turn_delay;

	if (gun_turn_tick == 0) {
		if (weapon_dir < dir)
			weapon_dir += 1;
		else if (weapon_dir > dir)
			weapon_dir -= 1;
	}
}

void Ball::updateWeapon() {
	if (status == STATE_LOCKED_VEHICLE) return;

	int yback = y;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			y -= hop_offsets[hop_idx];
			break;

		case STATE_JUMP:
		case STATE_FALL:
			if (dy < 0) y += dy - 2;
			break;
	}

	if (is_firing) {
		weapon_tick += 1;
		weapon_tick %= weapon_anim_delay;

		if (weapon_tick == 0) weapon_step += 1;

		if (weapon_step == weapon().stats.anim_steps) is_firing = false;
	}

	// A frame runs zero to several updates, so the press edge is taken here rather than per frame
	int step = ctrl->nextWeapon() ? 1 : ctrl->prevWeapon() ? -1 : 0;
	if (step && !switch_held_) cycleWeapon(step);
	switch_held_ = step != 0;

	if (!is_firing && ctrl->fire() && !level().rules.hold_fire.on) {
		is_firing = true;
		weapon_step = 0;
		weapon_tick = 0;

		weapon().pullTrigger();

		gun_fire_tick = weapon().stats.fire_interval - 1;
	}

	recoil_step += 1;
	recoil_step %= 5;

	gun_fire_tick += 1;
	gun_fire_tick %= weapon().stats.fire_interval;

	if (is_firing && gun_fire_tick == 0 && !(weapon().stats.turn_delay <= 1 && weapon_dir != dir)) {
		weapon().fire(*this, gunPose());

		if (!hasUnlimitedAmmo() && --ammo[weapon_id] == 0) {
			// The strongest weapon left takes over
			int id = NB_WEAPONS - 1;
			while (!carries(id)) id--;
			equip(id);
		}
	}

	y = yback;
}

void Ball::takeHit(const Shot*) {}

void Ball::takeHit(int damage) {
	if (invincible > 0 || status == STATE_DYING) return;

	hp -= damage;
	perfect = false;
	if (hp <= 0) {
		hp = 0;
		status = STATE_DYING;
		is_firing = false;
		weapon_step = weapon_tick = 0;
		anim_step = 0;
		anim_tick = 0;
		col_on = false;
		level().camera.frozen = true;
	} else {
		invincible = 200;
		hurt_timer = 100;
	}
}

void Ball::onDie() {
	if (dy < 0) dy = 0;

	fall();

	anim_tick += 1;
	anim_tick %= 7;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 15) {
		nb_life -= 1;

		if (nb_life == 0) {
			destroyed = true;
			level().camera.frozen = false;
			return;
		}

		status = STATE_COME_BACK;
		y_to_go = y;
		y = -50;
		col_on = true;
		hp = 5;
		weapon_dir = dir = BBDIR_RIGHT;
		invincible = 400;

		dropWeapons();
		rearm();

		pic = pbk_own.anim("jump_right")[2];
		dy = -5;  // So the eye and gun are drawn at jump height
		dx_jump = 0;
		is_firing = false;

		if (session().cow_bomb) nb_cow_bomb = 1;
	} else if (anim_step == 5 && anim_tick == 0 && level().terrain.platform_y(x, y) != 0) {
		if (dir <= BBLIM_RIGHT)
			engrave(x, y, pbk_own.pic("grave_right"));
		else
			engrave(x, y, pbk_own.pic("grave_left"));
	} else if (anim_step < 5) {
		if (dir <= BBLIM_RIGHT)
			pic = pbk_own.anim("die_right")[anim_step];
		else
			pic = pbk_own.anim("die_left")[anim_step];
	}
}

void Ball::renderDying() {
	if (anim_step < 5) Sprite::render();
}

void Ball::onComeBack() {
	level().camera.frozen = true;

	if (y < y_to_go) {
		step(3, 8);

		y += 5;

		pic = pbk_own.anim("spin")[weapon_id * 8 + anim_step];
	} else {
		y = y_to_go;
		status = STATE_NORMAL;
		anim_step = 0;
		anim_tick = 0;
		level().camera.frozen = false;
	}
}

void Ball::onSomersault() {
	int yp;

	step(3, 8);

	fall2();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		slide_delay = 200 * dx_jump;
		dy = 0;
		y = yp;
		onNormal();
		return;
	}

	if (ctrl->rightHeld() && dx_jump < 2)
		dx_jump = 2;
	else if (ctrl->leftHeld() && dx_jump > -2)
		dx_jump = -2;

	int xx = x + dx_jump;

	if (!level().terrain.solid_wall(xx, y) && xx > level().camera.offset + 20 && xx < level().camera.offset + 620) x = xx;

	pic = pbk_own.anim("spin")[weapon_id * 8 + anim_step];
}

bool Ball::okBonus() {
	// Every carried weapon counts, or switching to the M16 would farm bonuses
	for (int id = ID_M16 + 1; id < NB_WEAPONS; id++) {
		if (ammo[id] > weapons_[id]->stats.low_ammo) return false;
	}
	return true;
}

void Ball::setSuperWeapon() {
	if (id_ball == ID_BLIP || !level().rules.flamethrower_ok) {
		ammo[ID_LASER] = std::max(ammo[ID_LASER], 750);
		equip(ID_LASER);
	} else {
		ammo[ID_LF] = std::max(ammo[ID_LF], 1000);
		equip(ID_LF);
	}
}

void Ball::equip(int id) {
	weapon_id = id;

	weapon_step = 0;
	weapon_tick = 0;
	recoil_step = 0;
	is_firing = false;
}

void Ball::dropWeapons() {
	std::fill(std::begin(ammo), std::end(ammo), 0);
	equip(ID_M16);
}

void Ball::cycleWeapon(int step) {
	int id = weapon_id;
	do {
		id = (id + step + NB_WEAPONS) % NB_WEAPONS;
	} while (!carries(id));
	if (id != weapon_id) equip(id);
}

void Ball::colFromPic() {
	Sprite::colFromPic();

	if (status == STATE_SOMERSAULT) {
		x1 += 15;
		x2 -= 15;
		y1 += 15;
		y2 -= 15;
	} else {
		x1 += 10;
		x2 -= 10;
		y1 += 10;
		y2 -= 10;
	}
}

void Ball::lockVehicle(bool can_fire, bool can_dir) {
	status = STATE_LOCKED_VEHICLE;
	locked_fire = !can_fire;
	locked_dir = !can_dir;

	if (!can_fire) {
		is_firing = false;
		weapon_step = weapon_tick = 0;
	}
}

void Ball::unlockVehicle() {
	anim_step = anim_tick = 0;
	status = STATE_SOMERSAULT;
	dy = -5;
	dx_jump = 0;
	is_firing = false;
	weapon_step = weapon_tick = 0;
}

void Ball::onVehicle() {}

void Ball::renderVehicle() {
	if (dir <= BBLIM_RIGHT)
		pic = pbk_own.anim("walk_right")[0];
	else
		pic = pbk_own.anim("walk_left")[0];

	Sprite::render();
	renderEye(x, y);

	renderWeapon(x, y);
}
