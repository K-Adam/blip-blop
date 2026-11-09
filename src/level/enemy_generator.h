#pragma once

class EnemyGenerator {
   public:
	virtual ~EnemyGenerator() = default;
	int enemy_id;
	int facing;
	int x;
	int y;
	int period;
	int capacity;
	int t;
	bool destroyed;

	EnemyGenerator();

	virtual void update();

	inline virtual bool isDestroyed() { return destroyed; };
};
