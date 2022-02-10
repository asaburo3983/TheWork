#pragma once
#include "GameManager.h"
#include "GameSystem.h"
enum VectorNum {
	Left, Right, Up, Down
};
class PlayerAttackEffect {
public:
	int vectorNum = 0;
	Pos2 pos;//マス目上でのXY座標
	MODEL_DATA md[3];//エフェクト位置
	int count = 0;
	int countMax = 120;

	PlayerAttackEffect(MODEL_DATA player, Pos2 _pos, int _vectorNum) {
		vectorNum = _vectorNum;
		pos = _pos;
		count = 0;
		for (int i = 0; i < 3; i++) {

			md[i].scale = Vector3(0.35, 0.35, 0.35);
			//位置設定
			md[i].pos = player.pos;
			md[i].pos.Y -= 0.66f;
			int tmp = -1;
			if (vectorNum <= Right) {

				if (vectorNum == Right)tmp = 1;
				md[i].pos.X += (i + 1) * 0.66f * tmp;
			}
			else {
				if (vectorNum == Up)tmp = 1;
				md[i].pos.Z += (i + 1) * 0.66f * tmp;
			}

		}

	}
	void Updata() {
		GameSound* gs = GameSound::GetInstance();
		count += 4;
		float def[3] = { 20,15,10 };
		switch (count) {
		case 20:
		case 40:
		case 60:
			gs->Attack.Play(false);
			break;
		}
		if (count < 20) {
			md[0].pos.Y += 0.66f / def[0];
		}
		else if (count < 40) {
			md[1].pos.Y += 0.66f / def[1];
		}
		else if (count < 60) {
			md[2].pos.Y += 0.66f / def[2];
		}
		else if (count < 80) {
			md[0].pos.Y -= 0.66f / def[0];
		}
		else if (count < 100) {
			md[1].pos.Y -= 0.66f / def[1];
		}
		else if (count < 120) {
			md[2].pos.Y -= 0.66f / def[2];
		}


	}
};
class PlayerBomEffect {
public:
	MODEL_DATA md;//エフェクト位置(中心位置)
	int count = 0;
	int countMax = 60;
	std::vector <int> angle;
	float r = 0;
	float speed = 0.1f;
	PlayerBomEffect() {
		r = 0;
		count = 0;
		int max90 = 5;

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

class Player{
public:
	static Player* GetInstance() {
		static Player pl;
		return &pl;
	}
	//ステージ上でのマス目番号
	int x = 4;//中央
	int y = 4;
	int height = 0;//高さ

	int HP = 5;
	int BOM = 3;

	int hpCount = -1;
	const int hpCountMax = 30;

	int bomCount = -1;
	const int bomCountMax = 30;

	int power = 100;
	int speed = 6;


	int moveNum = 0;

	bool damage = false;
	int damageCount = 0;//無敵時間で使う
	int damageCountMax = 90;

	bool death = false;
	int deathCount = 0;
	//実際の座標
	MODEL_DATA md;
	//回転計の動く前の座標
	Vector3 pos = Vector3(0.0f, 0.65f, 0.0f);
	Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(0.3, 0.3, 0.3);

	std::vector<PlayerAttackEffect> playerAttackEffect;
	std::vector<PlayerBomEffect> playerBomEffect;
	void Reset();

	void Updata();

	void DrawShadow();
	void Draw();

};