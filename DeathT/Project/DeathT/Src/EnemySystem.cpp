#include "EnemySystem.h"

Item::Item(int _num,Vector3 _pos) {
	if (_num > 1) {
		_num = 1;
	}
	num = _num;
	md.pos = _pos;
	md.scale = Vector3(0.3, 0.3, 0.3);
}
void Item::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();//�O������v���C���[���̊m��
	md.degree.Y++;
	count++;
	if (count > countMax) {
		exist = false;
	}
	//�G���A�O�̂��̂͏���
	if (md.pos.X > 3 || md.pos.X < -3||
		md.pos.Z > 3 || md.pos.Z < -3 ||
		md.pos.Y > 4 || md.pos.Y < -3) {

		exist = false;
	}
	//�����Ă���
	if (md.pos.Y > 0.66) {
		md.pos.Y -= 0.05;
	}


	Pos3 center;
	center.p[0] = player->md.pos.X;
	center.p[1] = player->md.pos.Y;
	center.p[2] = player->md.pos.Z;
	Pos3 point;
	point.p[0] = md.pos.X;
	point.p[1] = md.pos.Y;
	point.p[2] = md.pos.Z;
	Pos3 size;
	size.p[0] = 1.2;
	size.p[1] = 0.66;
	size.p[2] = 1.2;
	if (HitRect3D(center, size, point)) {
		gm->gs->ItemGet.Play(false);
		switch (num) {
		case 0:
			if(player->HP<10)
				player->HP++;
			break;
		case 1:
			if (player->BOM < 10)
				player->BOM++;
			break;
		}
		exist = false;
	}
}
//�U�R�I���S�G�t�F�N�g�̏���
EnemyDeathEffect::EnemyDeathEffect(int _effectDataSize) {
	r = 0;
	count = 0;
	for (int i = 0; i < _effectDataSize; i++) {
		angle.push_back(GetRand(0, 90));
		angle.push_back(GetRand(90, 180));
		angle.push_back(GetRand(180, 270));
		angle.push_back(GetRand(270, 360));
	}

}
void EnemyDeathEffect::Updata() {
	count++;
	r += speed;
	//�폜�����i�A���O���j
	if (count >= countMax) {
		for (int i = 0; i < angle.size(); i++) {
			angle.pop_back();
		}
	}
}

//�U�R�I����
Enemy::Enemy(int _HP, float _speed, Pos2 _pos, int _movePattern) {
	death = false;
	r = 1;
	HP = _HP;
	speed = _speed;
	movePattern = _movePattern;
	death = false;
	float size = 0.27f;
	md.scale = Vector3(size, size, size);
	md.pos = Vector3(_pos.p[0], 0.65f, _pos.p[1]);
	startFrame = 0;
}
Enemy::Enemy(int _HP, float _speed, Vector3 pos, int _movePattern) {
	death = false;
	r = 1;
	HP = _HP;
	speed = _speed;
	movePattern = _movePattern;
	death = false;
	float size = 0.27f;
	md.scale = Vector3(size, size, size);
	md.pos = pos;
	startFrame = 0;
	if (movePattern == AntiGravity) {
		randTurn.X = GetRand(0, 2) - 2;
		randTurn.Y = GetRand(0, 2) - 2;
		randTurn.Z = GetRand(0, 2) - 2;

	}
}
Enemy::Enemy(int _HP, float _speed, Pos2 _pos, int _movePattern, float force,float _forceSpeed, int LR,int _startFrame) {
	death = false;
	r = 1;
	HP = _HP;
	speed = _speed;
	movePattern = _movePattern;
	death = false;
	float size = 0.27f;
	md.scale = Vector3(size, size, size);
	md.pos = Vector3(_pos.p[0], 0.65f, _pos.p[1]);

	startFrame = _startFrame;
	Player* player = Player::GetInstance();//�O������v���C���[���̊m��
	if (_movePattern == Homing_Bezier) {
		forceSpeed = _forceSpeed;
		forceA = force;
		forceB = 0;

		float c = md.pos.Z - player->md.pos.Z+2;

		float b = pow(pow(c, 2) / 2, 0.5);

		pointA = GetCirclePointPosiY(md.pos.X, md.pos.Z, b, 135 + (90*LR));//�Ȃ���悤�̓_
		
		pointB = GetCirclePointPosiY(pointA.p[0], pointA.p[1], 100, 135+(90*(1-LR)));//�ŏI�z�[�~���O�ʒu

	}
}
void Enemy::Updata() {
	Player* player = Player::GetInstance();//�O������v���C���[���̊m��

	
	if (movePattern == AntiGravity) {
		if (md.pos.Y > 1.5) {
			md.degree.X+=randTurn.X;
			md.degree.Y+=randTurn.Y;
			md.degree.Z+=randTurn.Z;
		}
	}
	else {
		md.degree.X++;
		md.degree.Y++;
		md.degree.Z++;
	}


	//�v���C���[�Ƃ̓����蔻��i�U�R�̎����U���j
	Pos3 center;
	center.p[0] = player->md.pos.X;
	center.p[1] = player->md.pos.Y;
	center.p[2] = player->md.pos.Z;
	Pos3 point;
	point.p[0] = md.pos.X;
	point.p[1] = md.pos.Y;
	point.p[2] = md.pos.Z;
	Pos3 size;
	size.p[0] = 1.2;
	size.p[1] = 0.66;
	size.p[2] = 1.2;
	if (HitRect3D(center, size, point)&&death==false&& player->damageCount==0) {
		death = true;
		player->damage = true;
	}

	//�v���C���[�U���G�t�F�N�g�Ƃ̓����蔻��
	for (int i = 0; i < player->playerAttackEffect.size(); i++) {
		for (int h = 0; h < 3; h++) {
			Pos3 center;
			center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
			center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
			center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
			Pos3 point;
			point.p[0] = md.pos.X;
			point.p[1] = md.pos.Y - 0.3;
			point.p[2] = md.pos.Z;
			Pos3 size;
			size.p[0] = 1.3;
			size.p[1] = 0.66;
			size.p[2] = 1.3;

			if (HitRect3D(center, size, point)) {
				HP -= player->power;
			}
		}
	}

	//�J�E���g������Ȃ��ꍇ�����𒆒f
	if (startFrame > countFrame) {
		countFrame++;
		return;
	}

	switch (movePattern) {
		//�v���C���[�Ǐ]
	case Homing://�����Ǐ]

		if (md.pos.X > player->md.pos.X) {
			md.pos.X -= speed;
		}
		else {
			md.pos.X += speed;
		}
		if (md.pos.Z > player->md.pos.Z) {
			md.pos.Z -= speed;
		}
		else {
			md.pos.Z += speed;
		}
		break;
	case Homing_Bezier:

		if (forceA > 0)
			forceA -= forceSpeed;

		forceB += forceSpeed;

		Pos2 my;
		my.p[0] = md.pos.X;
		my.p[1] = md.pos.Z;
		float ang;
		ang = GetAnglePosi(my, pointA);
		Pos2 posA = GetCirclePointPosiY(md.pos.X, md.pos.Z, forceA, ang);
		md.pos.X += posA.p[0] - md.pos.X;
		md.pos.Z += posA.p[1] - md.pos.Z;

		ang = GetAnglePosi(my, pointB);
		Pos2 posB = GetCirclePointPosiY(md.pos.X, md.pos.Z, forceB, ang);
		md.pos.X += posB.p[0] - md.pos.X;
		md.pos.Z += posB.p[1] - md.pos.Z;

		break;
		//�X�e�[�W���S�֌����� �g���ۂɂ�angle��r�̑�����K�v
	case RotationMove:

		angle++;
		r -= speed;
		Pos2 tmp = GetCirclePoint(0, 0, r, angle);
		md.pos.X = tmp.p[0];
		md.pos.Z = tmp.p[1];
		break;
	case LeftMove:
		md.pos.X -= speed;
		break;
	case RightMove:
		md.pos.X += speed;
		break;
	case UpMove:
		md.pos.Z += speed;
		break;
	case DownMove:
		md.pos.Z -= speed;
		break;

	case AntiGravity:
		md.pos.Y += speed;
		break;
	case Ring:
		r -= speed;
		Pos2 tmp2 = GetCirclePoint(0, 0, r, angle);
		md.pos.X = tmp2.p[0];
		md.pos.Z = tmp2.p[1];
		break;
	}

}


//�Q�[���S�̂̃U�R�I�����Ǘ�
void EnemySystem::Reset() {
	enemyDeleteNum = 0;
	//0 start1
	//135*60 Boss1
	//140*60 start2
	//275*60 Boss2
	// 300 * 60 start3
	//// 430 * 60 Boss3
	switch (GameSystem::GetInstance()->stageNum) {
	case 0://Start1
		gameCount = 0;
		break;
	case 1://Boss1
		gameCount = 134 * 60;
		break;
	case 2://Start2
		gameCount = 140 * 60;
		break;
	case 3://Boss2
		gameCount = 275 * 60;
		break;
	case 4://Start3
		gameCount = 300 * 60;
		break;
	case 5://Boss3
		gameCount = 430 * 60;
		break;
	default:
		gameCount = 0;
		break;
	}

	while (enemy.size() > 0) {
		enemy.pop_back();
	}
	while (enemyDeathEffect.size() > 0) {
		enemyDeathEffect.pop_back();
	}
	while (item.size() > 0) {
		item.pop_back();
	}

}
void EnemySystem::Updata() {
	GameManager* gm = GameManager::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();
	Player* player = Player::GetInstance();//�O������v���C���[���̊m��
	gameCount++;

	const int Seconds = 60;

	//�P�ʂ܂ł̃p�^�[��///////////////////////////////////////////
	//�����͊�{�I�ɂQ���ŏI��������
	if (gameCount < 30 * Seconds) {
		if (gameCount % (1 * Seconds) == 0) {
			float dist = 3;
			Pos2 tmpPos;
			tmpPos.p[0] = dist + (GetRand(0, 1) * -dist * 2);
			tmpPos.p[1] = dist + (GetRand(0, 1) * -dist * 2);

			Enemy ene(100, 0.005, tmpPos, Homing);
			enemy.push_back(ene);
		}
	}
	else if (gameCount < 60 * Seconds ) {
		if (gameCount % (1 * Seconds) == 0) {
			for (int i = 0; i < 2; i++) {
				float dist = 3;
				Pos2 tmpPos;
				tmpPos.p[0] = dist + (GetRand(0, 1) * -dist * 2);
				tmpPos.p[1] = dist + (GetRand(0, 1) * -dist * 2);

				Enemy ene(100, 0.005, tmpPos, Homing);
				enemy.push_back(ene);
			}
		}
	}
	else if (gameCount < 65 * Seconds) {
		//�ܕb�x�e
	}
	else if (gameCount < 80 * Seconds ) {
		if (gameCount % (1 * Seconds) == 0) {
			Enemy ene(100, 0.005, Pos2{ 4,4 }, RotationMove);
			ene.angle = 90 - 45;
			ene.r = 3;
			enemy.push_back(ene);
		}
	}
	else if (gameCount < 85 * Seconds) {
		//�ܕb�x�e
	}
	else if (gameCount < 100 * Seconds ) {
		if (gameCount % (1 * Seconds) == 0) {
			for (int i = 0; i < 2; i++) {
				Enemy ene(100, 0.005, Pos2{ 4,4 }, RotationMove);
				ene.angle = i * 180 - 45;
				ene.r = 4;
				enemy.push_back(ene);
			}
		}
	}
	else if (gameCount < 105 * Seconds) {
		//�ܕb�x�e
	}
	else if (gameCount < 120 * Seconds) {
		if (gameCount % (2 * Seconds) == 0) {
			for (int i = 0; i < 3; i++) {
				Enemy ene(100, 0.005, Pos2{ 4,4 }, RotationMove);
				ene.angle = i * 135;
				ene.r = 4;
				enemy.push_back(ene);
			}
		}
	}
	else if (gameCount < 130 * Seconds) {
		//10�b�x�e
	}
	else if (gameCount < 135 * Seconds) {
		//�{�X���o��
		gameCount = 0; //���G�o�������[�v������			
		bossSystem->boss1Exist = true;
		//�{�X���S���ɂ�140�b�܂ŃJ�E���g��������
	}
	/////////////////////////////////////////////////////////
	//�Q�ʂ���̃p�^�[��////////////////////////////////////
	else if (gameCount < 145 * Seconds) {

		//10�b�x�e
	}
	else if (gameCount < 155 * Seconds ) {
		if (gameCount % (1 * Seconds) == 0) {
			float dist = 0.66;
			Pos2 tmpPos;
			tmpPos.p[0] = dist * (GetRand(0, 8) - 4);
			tmpPos.p[1] = 4;

			Enemy ene(100, 0.05, tmpPos, DownMove);
			enemy.push_back(ene);
		}
	}
	else if (gameCount < 170 * Seconds  ) {
		if (gameCount % (1 * Seconds) == 0) {
			int rand = 100;
			for (int i = 0; i < 2; i++) {
				float dist = 0.66;
				Pos2 tmpPos;

				int rand2 = NULL;
				do {
					rand2 = (GetRand(0, 8) - 4);
				} while (rand2 == rand);

				tmpPos.p[0] = dist * rand2;
				tmpPos.p[1] = -4;

				Enemy ene(100, 0.05, tmpPos, UpMove);
				enemy.push_back(ene);
				rand = rand2;
			}
		}
	}
	else if (gameCount < 190 * Seconds ) {
		if (gameCount % (1 * Seconds) == 0) {
			int rand[3] = { 10,10,10 };
			for (int i = 0; i < 3; i++) {
				float dist = 0.66;
				Pos2 tmpPos;

				int rand2 = NULL;
				do {
					rand2 = (GetRand(0, 8) - 4);
				} while (rand2 == rand[0] || rand2 == rand[1]);

				tmpPos.p[0] = 4;
				tmpPos.p[1] = dist * rand2;

				Enemy ene(100, 0.05, tmpPos, LeftMove);
				enemy.push_back(ene);
				rand[i] = rand2;
			}
			rand[0] = 10;
			rand[1] = 10;
			rand[2] = 10;
			for (int i = 0; i < 3; i++) {
				float dist = 0.66;
				Pos2 tmpPos;

				int rand2 = NULL;
				do {
					rand2 = (GetRand(0, 8) - 4);
				} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

				tmpPos.p[0] = -4;
				tmpPos.p[1] = dist * rand2;
				Enemy ene(100, 0.05, tmpPos, RightMove);
				enemy.push_back(ene);
				rand[i] = rand2;
			}

		}
	}
	else if (gameCount < 210 * Seconds ) {
	if (gameCount % (1 * Seconds) == 0) {
		int rand[3] = { 10,10,10 };
		for (int i = 0; i < 3; i++) {
			float dist = 0.66;
			Pos2 tmpPos;

			int rand2 = NULL;
			do {
				rand2 = (GetRand(0, 8) - 4);
			} while (rand2 == rand[0] || rand2 == rand[1]);

			tmpPos.p[0] = 4;
			tmpPos.p[1] = dist * rand2;

			Enemy ene(100, 0.04, tmpPos, LeftMove);
			enemy.push_back(ene);
			rand[i] = rand2;
		}
		rand[0] = 10;
		rand[1] = 10;
		rand[2] = 10;
		for (int i = 0; i < 3; i++) {
			float dist = 0.66;
			Pos2 tmpPos;

			int rand2 = NULL;
			do {
				rand2 = (GetRand(0, 8) - 4);
			} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

			tmpPos.p[0] = dist * rand2;
			tmpPos.p[1] = 4;
			Enemy ene(100, 0.04, tmpPos, DownMove);
			enemy.push_back(ene);
			rand[i] = rand2;
		}

	}
	}
	else if (gameCount < 230 * Seconds) {
		if (gameCount % (30) == 0) {
			int rand[5] = { 10,10,10,10,10 };
			for (int i = 0; i < 5; i++) {
				float dist = 0.66;
				Pos2 tmpPos;

				int rand2 = NULL;
				do {
					rand2 = (GetRand(0, 8) - 4);
				} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2] || rand2 == rand[3]);

				tmpPos.p[0] = dist * rand2;
				tmpPos.p[1] = -4;

				Enemy ene(100, 0.05, tmpPos, UpMove);
				enemy.push_back(ene);
				rand[i] = rand2;
			}
		}
	}
	else if (gameCount < 250 * Seconds) {
		if (gameCount % (1 * Seconds) == 0) {
			int rand = (GetRand(0, 8) - 4);
			float dist = 0.66;
			Pos2 tmpPos;
			tmpPos.p[0] = dist * rand;

			float speed = 0.03;
			tmpPos.p[1] = -4;
			Enemy ene(100, speed, tmpPos, UpMove);
			enemy.push_back(ene);
			tmpPos.p[1] = 4;
			Enemy ene2(100, speed, tmpPos, DownMove);
			enemy.push_back(ene2);

			tmpPos.p[1] = dist * rand;
			tmpPos.p[0] = 4;
			Enemy ene3(100, speed, tmpPos, LeftMove);
			enemy.push_back(ene3);
			tmpPos.p[0] = -4;
			Enemy ene4(100, speed, tmpPos, RightMove);
			enemy.push_back(ene4);
		}
	}
	else if (gameCount < 270 * Seconds ) {
		if (gameCount % 90 == 0) {
			int rand = 100;
			for (int i = 0; i < 2; i++) {
				int rand2 = NULL;

				do {
					rand2 = (GetRand(0, 8) - 4);
				} while (rand2 == rand);


				float dist = 0.66;
				Pos2 tmpPos;
				tmpPos.p[0] = dist * rand2;

				float speed = 0.03;
				tmpPos.p[1] = -4;
				Enemy ene(100, speed, tmpPos, UpMove);
				enemy.push_back(ene);
				tmpPos.p[1] = 4;
				Enemy ene2(100, speed, tmpPos, DownMove);
				enemy.push_back(ene2);

				tmpPos.p[1] = dist * rand2;
				tmpPos.p[0] = 4;
				Enemy ene3(100, speed, tmpPos, LeftMove);
				enemy.push_back(ene3);
				tmpPos.p[0] = -4;
				Enemy ene4(100, speed, tmpPos, RightMove);
				enemy.push_back(ene4);
				rand = rand2;
			}
		}
	}
	else if (gameCount < 280 * Seconds) {
		//10�b�x�e
	}
	else if (gameCount < 289 * Seconds) {
		if (gameCount % (90) == 0) {
			int lrud1= GetRand(0,3);
			int lrud2 = -1;
			do {
				lrud2=GetRand(0, 3);
			} while (lrud1 == lrud2);
		

			int rand = (GetRand(0, 8) - 4);
			float dist = 0.66;
			Pos2 tmpPos;
			tmpPos.p[0] = dist * rand;

			float speed = 0.03;
			tmpPos.p[1] = -4;
			Enemy ene(100, speed, tmpPos, UpMove);
			if(lrud1==0|| lrud2 == 0)
				enemy.push_back(ene);
			tmpPos.p[1] = 4;
			Enemy ene2(100, speed, tmpPos, DownMove);
			if (lrud1 == 1 || lrud2 ==1)
				enemy.push_back(ene2);

			tmpPos.p[1] = dist * rand;
			tmpPos.p[0] = 4;
			Enemy ene3(100, speed, tmpPos, LeftMove);
			if (lrud1 == 2 || lrud2 == 2)
				enemy.push_back(ene3);
			tmpPos.p[0] = -4;
			Enemy ene4(100, speed, tmpPos, RightMove);
			if (lrud1 == 3 || lrud2 == 3)
				enemy.push_back(ene4);
		}
	}
	else if (gameCount < 290 * Seconds) {
		//�{�X���o��
		
		bossSystem->boss2Exist = true;
		gameCount = 281 * Seconds; //���G�o�������[�v������			



	}
	else if (gameCount < 295 * Seconds) {
		//�{�X�Q��|������͂��̕b���Ɉړ�(295*seconds)
	}
	//420�܂ł��
	else if (gameCount < 300 * Seconds) {
	}
	else if (gameCount < 320 * Seconds) {
		if (gameCount % 90 == 0) {
			const int size = 3;
			int rand[size][size][2]{
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}}
			};
			float dist = 0.66;
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int randY = 0;
					int randX = 0;
					bool loop = true;
					while (loop) {
						randY = GetRand(0, 8);
						randX = GetRand(0, 8);
						loop = false;
						for (int _y = 0; _y < 4; _y++) {
							for (int _x = 0; _x < 4; _x++) {

								if (rand[_y][_x][0] == randY && rand[_y][_x][1] == randX) {
									loop = true;
								}
							}
						}
					}
					rand[y][x][0] = randY;
					rand[y][x][1] = randX;

					float rx = (randX - 4) * dist;
					float rz = (randY - 4) * dist;
					Enemy ene(100, 0.02, Vector3(rx, -3, rz), AntiGravity);
					//ene.md.degree.X = randX * 40;
					//ene.md.degree.Z = randY * 40;

					enemy.push_back(ene);
				}
			}
		}
	}
	else if (gameCount < 330 * Seconds ) {
		if (gameCount % 90 == 0) {
			const int size = 4;
			int rand[size][size][2]{
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1}}
			};
			float dist = 0.66;
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int randY = 0;
					int randX = 0;
					bool loop = true;
					while (loop) {
						randY = GetRand(0, 8);
						randX = GetRand(0, 8);
						loop = false;
						for (int _y = 0; _y < 4; _y++) {
							for (int _x = 0; _x < 4; _x++) {

								if (rand[_y][_x][0] == randY && rand[_y][_x][1] == randX) {
									loop = true;
								}
							}
						}
					}
					rand[y][x][0] = randY;
					rand[y][x][1] = randX;

					float rx = (randX - 4) * dist;
					float rz = (randY - 4) * dist;
					Enemy ene(100, 0.02, Vector3(rx, -3, rz), AntiGravity);


					enemy.push_back(ene);
				}
			}
		}
	}
	else if (gameCount < 360 * Seconds ) {
		if (gameCount % 90 == 0) {
			const int size = 5;
			int rand[size][size][2]{
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}
			};
			float dist = 0.66;
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int randY = 0;
					int randX = 0;
					bool loop = true;
					while (loop) {
						randY = GetRand(0, 8);
						randX = GetRand(0, 8);
						loop = false;
						for (int _y = 0; _y < 4; _y++) {
							for (int _x = 0; _x < 4; _x++) {

								if (rand[_y][_x][0] == randY && rand[_y][_x][1] == randX) {
									loop = true;
								}
							}
						}
					}
					rand[y][x][0] = randY;
					rand[y][x][1] = randX;

					float rx = (randX - 4) * dist;
					float rz = (randY - 4) * dist;
					Enemy ene(100, 0.02, Vector3(rx, -3, rz), AntiGravity);

					enemy.push_back(ene);
				}
			}
		}
	}
	else if (gameCount < 365 * Seconds) {
	//�T�b�x�e
	}
	else if (gameCount < 380 * Seconds) {
		if (gameCount % 90 == 0) {
			for (int i = 0; i < 8; i++) {
				int ang = 45;
				if (gameCount % 180 == 0) {
					ang += 22;
				}
				Pos2 anser = GetCirclePointPosiY(0, 0, 5, i * ang);
				Enemy ene(100, 0.02, anser, Ring);
				ene.angle = i * ang;
				ene.r = 5;
				enemy.push_back(ene);
			}
		}
	}
	else if (gameCount < 400 * Seconds ) {
		if (gameCount % 45 == 0) {
			for (int i = 0; i < 8; i++) {
				int ang = 45;
				if (gameCount % 90 == 0) {
					ang += 22;
				}
				Pos2 anser = GetCirclePointPosiY(0, 0, 5, i * ang);
				Enemy ene(100, 0.02, anser, Ring);
				ene.angle = i * ang;
				ene.r = 5;
				enemy.push_back(ene);
			}
		}
	//�����O�{
	}
	else if (gameCount < 420 * Seconds) {
		if (gameCount % 90 == 0) {
			const int size = 3;
			int rand[size][size][2]{
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}}
			};
			float dist = 0.66;
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int randY = 0;
					int randX = 0;
					bool loop = true;
					while (loop) {
						randY = GetRand(0, 8);
						randX = GetRand(0, 8);
						loop = false;
						for (int _y = 0; _y < 4; _y++) {
							for (int _x = 0; _x < 4; _x++) {

								if (rand[_y][_x][0] == randY && rand[_y][_x][1] == randX) {
									loop = true;
								}
							}
						}
					}
					rand[y][x][0] = randY;
					rand[y][x][1] = randX;

					float rx = (randX - 4) * dist;
					float rz = (randY - 4) * dist;
					Enemy ene(100, 0.02, Vector3(rx, -3, rz), AntiGravity);
					//ene.md.degree.X = randX * 40;
					//ene.md.degree.Z = randY * 40;

					enemy.push_back(ene);
				}
			}
			for (int i = 0; i < 8; i++) {
				int ang = 45;
				if (gameCount % 180 == 0) {
					ang += 22;
				}
				Pos2 anser = GetCirclePointPosiY(0, 0, 5, i * ang);
				Enemy ene(100, 0.02, anser, Ring);
				ene.angle = i * ang;
				ene.r = 5;
				enemy.push_back(ene);
			}
			//�����O���O�����B�e�B�@
		}
	}
	else if (gameCount < 430 * Seconds) {

	}
	else if (gameCount < 1000 * Seconds) {
		
		bossSystem->boss3Exist = true;

		if (gameCount % 90 == 0) {
			const int size = 3;
			int rand[size][size][2]{
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}},
				{{-1,-1},{-1,-1},{-1,-1}}
			};
			float dist = 0.66;
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int randY = 0;
					int randX = 0;
					bool loop = true;
					while (loop) {
						randY = GetRand(0, 8);
						randX = GetRand(0, 8);
						loop = false;
						for (int _y = 0; _y < 4; _y++) {
							for (int _x = 0; _x < 4; _x++) {

								if (rand[_y][_x][0] == randY && rand[_y][_x][1] == randX) {
									loop = true;
								}
							}
						}
					}
					rand[y][x][0] = randY;
					rand[y][x][1] = randX;

					float rx = (randX - 4) * dist;
					float rz = (randY - 4) * dist;
					Enemy ene(100, 0.02, Vector3(rx, -3, rz), AntiGravity);
					//ene.md.degree.X = randX * 40;
					//ene.md.degree.Z = randY * 40;

					enemy.push_back(ene);
				}
			}
		}
	}
	else if (gameCount < 1200 * Seconds) {
	gameCount = 435 * Seconds;
		//�N���A
	}

	//�U�R�G�ƃv���C���[�̓����菈���i�U�R�G�̍U���j
	GameSound* gs = GameSound::GetInstance();
	//�U�R�G�̏���
	for (int i = 0; i < enemy.size(); i++) {
		enemy[i].Updata();
		 
		//�t�B�[���h�O�ɍs���قǓ����ɂȂ� �C���X�^���X���̉e���œn���ɂ���
		//if (enemy[i].md.pos.X < -4 || enemy[i].md.pos.X > 4) {
		//	enemy[i].md.multiplyColor.w = 5.0f - abs(enemy[i].md.pos.X);
		//}
		//else if (enemy[i].md.pos.Y < -6 || enemy[i].md.pos.Y > 6) {
		//	enemy[i].md.multiplyColor.w = 7.0f - abs(enemy[i].md.pos.Y);
		//}
		//else if (enemy[i].md.pos.Z < -4 || enemy[i].md.pos.Z > 4) {
		//	enemy[i].md.multiplyColor.w = 5.0f - abs(enemy[i].md.pos.Z);
		//}

		//�폜���� �G�t�F�N�g���� �v���C���[�̍U��
		if (enemy[i].HP <= 0 || enemy[i].death == true) {
			gs->EnemyDead.Play(false);
			//���S�G�t�F�N�g�̍쐬
			EnemyDeathEffect edf(enemy[i].effectDataSize);
			edf.md.pos = enemy[i].md.pos;
			enemyDeathEffect.push_back(edf);
			//�A�C�e���̍쐬
			int itRand = GetRand(0, 99);
			if (player->HP+ player->BOM <= 2) {//�s���`�̎��P�O���ŃA�C�e��
				if (itRand < 10) {
					Item it(itRand/5, enemy[i].md.pos);
					item.push_back(it);
				}
			}
			else if (player->HP + player->BOM <= 7) {//���ʂ̎���4%�ŃA�C�e�����h���b�v����
				if (itRand < 4) {
					Item it(itRand / 2, enemy[i].md.pos);
					item.push_back(it);
				}
			}
			else if(player->HP + player->BOM > 7){//���C�Ȏ���2%�Ńh���b�v
				if (itRand <= 1) {
					Item it(itRand, enemy[i].md.pos);
					item.push_back(it);
				}
			}
			//�G�̍폜
			enemy.erase(enemy.begin() + i);

			enemyDeleteNum++;
		}
		//�G�t�F�N�g�Ȃ� ��ʊO�ɏo����
		else if (enemy[i].md.pos.X < -5 || enemy[i].md.pos.X > 5 ||
			enemy[i].md.pos.Y < -7 || enemy[i].md.pos.Y > 7||
			enemy[i].md.pos.Z < -5 || enemy[i].md.pos.Z > 5) {
				

			//�G�̍폜
			enemy.erase(enemy.begin() + i);
		}
	}
	//�G�t�F�N�g�̏���
	for (int i = 0; i < enemyDeathEffect.size(); i++) {
		enemyDeathEffect[i].Updata();

		if (enemyDeathEffect[i].count >= enemyDeathEffect[i].countMax) {
			enemyDeathEffect.erase(enemyDeathEffect.begin() + i);
		}
	}
	//�A�C�e���̏���
	for (int i = 0; i < item.size(); i++) {
		item[i].Updata();

		if (item[i].exist==false) {
			item.erase(item.begin() + i);
		}
	}
}
void EnemySystem::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	//�U�R�G�̉e
	int instCount = 0;
	for (int i = 0; i < enemy.size(); i++) {
		gm->gm->Enemy[0]->SetModelData(&enemy[i].md);
		gm->gm->Enemy[0]->SetWorldMatrix();
		//gm->gm->Enemy[0]->RenderShadowMap(&enemy[i].md);
		instCount++;
	}

	if (instCount != 0) {
		gm->gm->Enemy[0]->CreateWorldMatrixBuffer(instCount);
		gm->gm->Enemy[0]->RenderSMInstance(instCount);
	}
}

void EnemySystem::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();

	int instCount = 0;
	for (int i = 0; i < enemy.size(); i++) {
		gm->gm->Enemy[gameSystem->stageNum / 2]->SetModelData(&enemy[i].md);
		gm->gm->Enemy[gameSystem->stageNum / 2]->SetWorldMatrix();
		instCount++;

		if (enemy[i].md.pos.Y<1&&enemy[i].movePattern == AntiGravity) {
			MODEL_DATA md = enemy[i].md;
			md.scale = Vector3(0.35, 0.35, 0.35);
			md.degree = Vector3(0, 0, 0);
			md.pos.Y = 0.001;
			md.multiplyColor.w = 0.75;
			
			gm->gm->DangerBlock->RenderShadowModel(&md);
		}
	}
	//�U�R�G�j��G�t�F�N�g�@�Q�[���̐i�s�x�ŐF��ς���@�΁��������D
	MODEL_DATA md;
	for (int i = 0; i < enemyDeathEffect.size(); i++) {
		md.pos.Y = enemyDeathEffect[i].md.pos.Y;
		float size = 0.15;
		if (gameSystem->pause==false) {
			md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			size = (float)GetRand(100, 200) / 1000.0f;
		}
		else {
			md.degree = Vector3(i * 30, i * 30, i * 30);
		}
		md.scale = Vector3(size, size, size);
		for (int h = 0; h < enemyDeathEffect[i].angle.size(); h++) {
			
			Pos2 tmp = GetCirclePoint(enemyDeathEffect[i].md.pos.X, enemyDeathEffect[i].md.pos.Z, enemyDeathEffect[i].r, enemyDeathEffect[i].angle[h]);
			md.pos.X = tmp.p[0];
			md.pos.Z = tmp.p[1];

			gm->gm->Enemy[gameSystem->stageNum / 2]->SetModelData(&md);
			gm->gm->Enemy[gameSystem->stageNum / 2]->SetWorldMatrix();
			instCount++;
		}
	}
	//�A�C�e���̕`��
	for (int i = 0; i < item.size(); i++) {
		gm->gm->Item[item[i].num]->RenderShadowModel(&item[i].md);
	}
	if (instCount != 0) {
		gm->gm->Enemy[gameSystem->stageNum / 2]->CreateWorldMatrixBuffer(instCount);
		gm->gm->Enemy[gameSystem->stageNum / 2]->RenderSInstance(instCount);
	}
}
void  EnemySystem::AddEnemy(Enemy ene) {

	enemy.push_back(ene);
}
void  EnemySystem::AddEnemy(int _HP, float _speed, Pos2 _pos, int _movePattern) {
	Enemy ene(_HP, _speed, _pos, _movePattern);
	enemy.push_back(ene);
}
void  EnemySystem::AddEnemy(int _HP, float _speed, Pos2 _pos, int _movePattern, float force, float _forceSpeed, int LR, int startFrame){
	Enemy ene(_HP, _speed, _pos, _movePattern,  force,  _forceSpeed, LR, startFrame);
	enemy.push_back(ene);
}