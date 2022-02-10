#pragma once
#include "GameManager.h"
#include "Player.h"

class Stage {
public:
	int playerWin[2] = { 3,3 };
	
	static Stage* GetInstance() {
		static Stage st;
		return &st;
	}
	bool moveArea[9][9];

	bool moveArea1[9][9]{
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
	};
	bool moveArea2[9][9]{
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,0,1,1,1,0,1,1},
		{1,1,0,1,1,1,0,1,1},
		{1,1,0,1,0,1,0,1,1},
		{1,1,0,1,1,1,0,1,1},
		{1,1,0,1,1,1,0,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
	};
	bool moveArea3[9][9]{
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1},
	};
	
	void Reset(int stageNum);

};

class PlayerDeathEffect {
public:
	MODEL_DATA md;//�G�t�F�N�g�ʒu(���S�ʒu)
	int count = 0;
	int countMax = 60;
	std::vector <int> angle;
	float r = 0;
	float speed = 0.4f;
	PlayerDeathEffect() {
		r = 0;
		count = 0;
		int max90 = 10;

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
		//�폜�����i�A���O���j
		if (count >= countMax) {
			for (int i = 0; i < angle.size(); i++) {
				angle.pop_back();
			}
		}
	}
};

class PlayerBase {
public:
	int playerNum = 0;
	//�X�e�[�W��ł̃}�X�ڔԍ�
	int x = 4;//����
	int y = 4;
	int height = 0;//����

	
	int power = 100;
	int speed = 6;


	int moveNum = 0;

	bool death = false;

	int invincibleCount = 0;
	const int invincibleCountMax = 120;


	std::vector<PlayerAttackEffect> playerAttackEffect;
	std::vector<PlayerDeathEffect> playerDeathEffect;
	//���ۂ̍��W
	MODEL_DATA md;
	//��]�v�̓����O�̍��W
	Vector3 pos = Vector3(0.0f, 0.65f, 0.0f);
	Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(0.3, 0.3, 0.3);

	void ResetBase();
	void UpdataBase();
	void DrawShadowBase();
	void DrawBase();

	virtual void Reset()=0;
	virtual void Updata()=0;
	virtual void DrawShadow()=0;
	virtual void Draw()=0;
};
class Player1 : public PlayerBase{
public:
	static Player1* GetInstance() {
		static Player1 pl;
		return &pl;
	}





	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();

};
class Player2 : public PlayerBase{
public:
	static Player2* GetInstance() {
		static Player2 pl;
		return &pl;
	}


	void Reset();
	void Updata();
	void DrawShadow();
	void Draw();

};