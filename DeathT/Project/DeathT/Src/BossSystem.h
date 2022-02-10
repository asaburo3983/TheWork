#pragma once
#include "GameManager.h"
#include "Player.h"
#include "BossSystem.h"
#include "GameSystem.h"
#include "EnemySystem.h"
class Boss {
public:
	MODEL_DATA md;
	int animeNum = 5;
	int HP_MAX = 2500;
	int HP = 2500;
	int attackCount = 0;
	int attackCountMax = 180;//攻撃間隔
	int animeCount = 0;

	bool death = false;
	int deathCount = 0;

	int damageCount = 0;

public:

	virtual void Reset() = 0;
	virtual void Updata() = 0;
	virtual void DrawShadow() = 0;
	virtual void Draw() = 0;
};

class Boss1WaveDeathEffect {
public:
	MODEL_DATA md;//エフェクト位置(中心位置)
	int count = 0;
	int countMax = 10;
	std::vector <int> angle;
	float r = 0;
	float speed = 0.1f;
	Boss1WaveDeathEffect() {
		r = 0;
		count = 0;
		int max90 = 2;

		for (int i = 0; i < max90; i++) {
			angle.push_back(GetRand(0, 90));
			angle.push_back(GetRand(90, 180));
			angle.push_back(GetRand(180, 270));
			angle.push_back(GetRand(270, 360));
		}
	}
	void Updata() {
		count++;
		r += speed;
		//削除処理（アングル）
		if (count >= countMax) {
			for (int i = 0; i < angle.size(); i++) {
				angle.pop_back();
			}
		}
	}
};

class Boss1Wave {
public:
	MODEL_DATA md;
	float speed = 0.005;
	int x = 4;
	int z = 4;
	float r = 0;
	Boss1Wave(float _speed, int _x, int _z, float _boss1PosZ) {
		speed = _speed;
		x = _x;
		z = _z;
		r = 0;
		md.pos.Z = _boss1PosZ;
		float s = 0.35f;
		md.scale = Vector3(s, s, s);
		md.pos.Y = 7;
	}
	void Updata() {
		r += speed;
		md.pos.X = (x - 4) * 0.66;
		md.pos.Z = (z - 4) * 0.66;
		if (r > 5.0f) {

			if (md.pos.Y > 0.66) {
				md.pos.Y -= 0.08f;
			}

		}
	}
};
class Boss1 : public Boss{
public:

	float tackleSpeed = 0.4f;
	int animeConstFrame[4] = {
		100,120,140,60
	};

	std::vector<Boss1Wave> boss1Wave;
	std::vector<Boss1WaveDeathEffect> boss1WaveDeathEffect;
	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();
	/*
	0　死亡		60F
	1 正面殴り 100F
	2 なし
	3 待機　 60F
	4 タックル 120F
	5 降り下ろし 140F
	*/
};


class Boss2 : public Boss{
public:

	int animeConstFrame[7] = {
		70,60,60,90,90,60,NULL
	};
public:
	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();

};



class Boss3 : public Boss {
public:

	int animeConstFrame[8] = {
	70,70,75,75,100,90,60,30
	};
public:
	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();

};

class BossSystem{
public:
	Boss1 boss1;
	Boss2 boss2;
	Boss3 boss3;
	bool boss1Exist = false;
	bool boss2Exist = false;
	bool boss3Exist = false;

public:

	static BossSystem* GetInstance() {
		static BossSystem bs;
		return &bs;
	}

	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();
};