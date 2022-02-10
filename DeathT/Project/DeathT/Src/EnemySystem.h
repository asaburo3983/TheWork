#pragma once
#include "GameManager.h"
#include "Player.h"
#include "BossSystem.h"
#include "GameSystem.h"

using namespace GameMath;
enum MovePattern {
	Homing, RotationMove, LeftMove, RightMove, UpMove, DownMove, Homing_Bezier, Ring, AntiGravity
};

class Item {
public:
	bool exist = true;
	int num = 0;
	MODEL_DATA md;
	int count = 0;
	int countMax = 600;
	Item(int _num, Vector3 _pos);
	void Updata();
};
class EnemyDeathEffect {
public:
	MODEL_DATA md;//エフェクト位置(中心位置)
	int count = 0;
	int countMax = 120;
	std::vector <int> angle;
	float r = 0;
	float speed = 0.3f;
public:
	EnemyDeathEffect(int _effectDataSize);
	void Updata();
};

class Enemy {
public:
	Vector3 randTurn;
	int HP = 100;
	float speed = 0.005;
	MODEL_DATA md;

	float r = 5;
	float angle = 0;
	int movePattern = 0;
	bool death = false;

	//ホーミングミサイル用
	float forceSpeed = 0.1;
	Pos2 pointA = {0,0};//プレイヤーと自分の間の４５度地点を取る
	float forceA=0;
	Pos2 pointB = {0,0};//プレイヤーのXとZを格納
	float forceB=0;

	int startFrame = 0;
	int countFrame = 0;//startFrameになったら処理を開始する

	int effectDataSize = 4;

public:
	Enemy(int _HP, float _speed, Pos2 _pos, int _movePattern);
	Enemy(int _HP, float _speed, Pos2 _pos, int _movePattern, float force, float _forceSpeed, int LR,int _startFrame);
	Enemy(int _HP, float _speed, Vector3 pos, int _movePattern);
	void Updata();
};

class EnemySystem {
public:
	//0 start1
	//135*60 Boss1
	//140*60 start2
	//275*60 Boss2
	// 300 * 60 start3
	//// 430 * 60 Boss3
	int gameCount = 0;
	std::vector<Enemy> enemy;
	std::vector<EnemyDeathEffect> enemyDeathEffect;
	std::vector<Item> item;

	int enemyDeleteNum=0;
public:
	static EnemySystem* GetInstance() {
		static EnemySystem es;
		return &es;
	}
	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();
	void AddEnemy(Enemy ene);
	void AddEnemy(int _HP, float _speed, Pos2 _pos, int _movePattern);
	void AddEnemy(int _HP, float _speed, Pos2 _pos, int _movePattern, float force, float _forceSpeed, int LR,int startFrame);
};