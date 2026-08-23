#pragma once

// First indices of the sprite groups in the shared picture banks. Code adds the direction or frame to them

// pbk_bb, sprites both players share
namespace BB {
enum : int {
	EYE_NORMAL = 0,		 // + facing 0..5
	EYE_ANGRY = 6,		 // + facing 0..5
	FLASH_M16 = 12,		 // + frame, points down, drawn turned to the gun
	FLASH_SHOTGUN = 15,	 // + frame, points down, drawn turned to the gun
	SHOT_M16 = 18,		 // points down, drawn rotated to the shot angle
	SHOT_PELLET = 19,	 // points down, drawn rotated to the shot angle
	LASER_MUZZLE = 20,	 // + frame, points down, drawn turned to the gun
	LASER_BEAM_H = 24,	 // + frame
	LASER_GLOW = 28,	 // + frame, points down, drawn turned to the gun
	HEALTH_EMPTY = 32,	 // + piece 0..4
	HEALTH_FULL = 37,	 // + piece 0..4
	COW_FALL = 42,		 // + frame
	COW_SPLAT_RIGHT = 50,
	COW_SPLAT_LEFT = 51,
	EXPLOSION_A = 52,  // + frame
	EXPLOSION_B = 59,  // + frame
	EXPLOSION_C = 71,  // + frame
};
}

// pbk_blip and pbk_blop share this layout
namespace BALL {
enum : int {
	WALK_RIGHT = 0,
	WALK_LEFT = 3,
	JUMP_RIGHT = 6,
	JUMP_LEFT = 9,
	DIE_RIGHT = 12,	 // + frame
	GRAVE_RIGHT = 16,
	DIE_LEFT = 17,	// + frame
	GRAVE_LEFT = 21,
	EYE_HURT_RIGHT = 22,
	EYE_HURT_LEFT = 23,
	GUN_M16 = 24,		 // + gun sector
	GUN_LASER = 34,		 // + gun sector
	GUN_SHOTGUN = 44,	 // + gun sector
	GUN_LF = 54,		 // + gun sector
	GUN_PM = 64,		 // + gun sector
	FLASH_PM = 74,		 // + gun sector * 3 + frame
	SHOTGUN_HAND = 104,	 // + gun sector % 5
	SPIN = 109,			 // + weapon * 8 + frame
};
}

// pbk_misc, bonuses, HUD icons and ambient effects
namespace MISC {
enum : int {
	BONUS_CORNERS_ON = 0,  // + corner 0..3
	BONUS_CORNERS_OFF = 4,
	ICON_PM = 8,
	ICON_SHOTGUN = 9,
	ICON_LASER = 10,
	ICON_LF = 11,
	BONUS_INVINCIBLE = 12,
	BONUS_BEER = 13,
	ICON_M16 = 14,
	SPLATTER = 15,	// + direction * 4 + frame
	BONUS_BARREL = 47,
	BONUS_COW = 48,
	ICON_COW_BOMB = 49,
	BLOOD_STAIN = 50,  // + 0..13
	RAINDROP = 65,	   // + size 0..3
	SNOWFLAKE_SMALL = 71,
	SNOWFLAKE_LARGE = 72,
	LASER_IMPACT = 74,	// + frame
	BUBBLE = 78,		// + 0..2
	GO_ARROW = 81,		// + frame
	GORE_DROP = 86,		// + 0..6 + offset
};
}
