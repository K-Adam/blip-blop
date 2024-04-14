#pragma once

class EnemyGenerator {
   public:
	virtual ~EnemyGenerator() = default;
	int id_ennemi;
	int sens;
	int x;
	int y;
	int periode;
	int capacite;
	int t;
	bool a_detruire;

	EnemyGenerator();

	virtual void update();

	inline virtual bool aDetruire() { return a_detruire; };
};
