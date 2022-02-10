#include "BossSystem.h"

void Boss1::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 0, 3);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 5;
	HP_MAX = 3000;//����HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//�U���Ԋu
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss1::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//��莞�Ԃ����Ƀv���C���[�̈ʒu�����čs�������߂�
		if (animeNum == 3 || animeNum == 7 || animeNum == 8) {
			attackCount++;
		}
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;
			bool tackleAttack = false;
			bool frontAttack = false;
			bool waveAttack = false;

			if (player->x < 6 && player->x>2 && player->y > 4) {
				tackleAttack = true;
			}
			else if (player->x < 6 && player->x>2 && player->y <= 4) {
				frontAttack = true;
			}

			int parsent = GetRand(1, 100);
			if (frontAttack) {
				animeNum = 1;

			}
			else if (tackleAttack) {
				animeNum = 4;

			}
			else {
				animeNum = 5;
			}
			if (parsent <= 5) {
				animeNum = 5;
			}

			if (animeNum == 5) {

			}
		}

		//�^�b�N���̍��W�߂�
		if (animeNum != 4) {
			if (md.pos.Z < 3) {
				md.pos.Z += 0.2f;
				if (md.pos.Z > 3) {
					md.pos.Z = 3;
				}
			}
			else if (md.pos.Z > 3) {
				md.pos.Z -= 0.2f;
				if (md.pos.Z < 3) {
					md.pos.Z = 3;
				}
			}

		}
		switch (animeNum) {
		case 3://STAY
			break;
		case 1://FRONT_ATTACK
			animeCount++;
			//�v���C���[�ւ̃_���[�W����
			if (animeCount == animeConstFrame[0] / 2) {
				gm->gs->Boss1FrontAttack.Play(false);
				if (player->x <= 6 && player->x >= 2 && player->y <= 4&&player->damageCount==0) {
					player->damage = true;
				}
			}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[0]) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		case 4://TACKLE_ATTACK
			if (animeCount == 60) {
				gm->gs->Boss1Tackle.Play(false);
			}
			if (animeCount > 60) {
				md.pos.Z -= tackleSpeed;

			}
			animeCount++;
			//�v���C���[�ւ̃_���[�W����
			Pos3 center;
			center.p[0] = player->md.pos.X;
			center.p[1] = player->md.pos.Y;
			center.p[2] = player->md.pos.Z;
			Pos3 point;
			point.p[0] = md.pos.X;
			point.p[1] = md.pos.Y;
			point.p[2] = md.pos.Z;
			Pos3 size;
			size.p[0] = 3.3;
			size.p[1] = 3.3;
			size.p[2] = 0.33;

			if (HitRect3D(center, size, point) && player->damageCount == 0) {
				player->damage = true;
			}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[1]) {
				animeCount = 0;
				animeNum = 3;
				md.pos.Z = 10;
			}
			break;
		case 5://WAVE_ATTACK
			animeCount++;
			if (animeCount == 100) {
				gm->gs->Boss1Avalanche.Play(false);
				Pos2 bossPos;
				bossPos.p[0] = md.pos.X;
				bossPos.p[1] = md.pos.Z;

				int tmp = GetRand(0, 1);
				for (int i = 0; i < 10; i++) {

					int randX = GetRand(0, 8);
					int randZ = GetRand(0, 8);

					Boss1Wave b1w(0.06f, randX, randZ, md.pos.Z);
					boss1Wave.push_back(b1w);

				}
			}
			if (animeCount >= animeConstFrame[2]) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		case 7://�_���[�W���󂯂���
		case 8:
			animeCount++;
			if (animeCount >= 40) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		}

		//�v���C���[�U���G�t�F�N�g�Ƃ̓����蔻��
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;
					point.p[1] = md.pos.Y;
					point.p[2] = md.pos.Z;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);
						if (animeNum == 3) {

							animeNum = 7 + GetRand(0, 1);
						}
						break;

					}
				}
			}
		}
		//���G����
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//�E�F�[�u�U���X�V
		for (int i = 0; i < boss1Wave.size(); i++)
		{
			boss1Wave[i].Updata();
			if (boss1Wave[i].r > 10) {
				Boss1WaveDeathEffect b1wde;
				b1wde.md = boss1Wave[i].md;

				Pos3 center;
				center.p[0] = player->md.pos.X;
				center.p[1] = player->md.pos.Y;
				center.p[2] = player->md.pos.Z;
				Pos3 point;
				point.p[0] = b1wde.md.pos.X;
				point.p[1] = b1wde.md.pos.Y;
				point.p[2] = b1wde.md.pos.Z;
				Pos3 size;
				size.p[0] = 0.66;
				size.p[1] = 0.66;
				size.p[2] = 0.66;
				if (HitRect3D(center, size, point)) {
					player->damage = true;
				}
				//�E�F�[�u���G�t�F�N�g�ǉ�
				boss1WaveDeathEffect.push_back(b1wde);
				//�E�F�[�u�폜
				boss1Wave.erase(boss1Wave.begin() + i);

			}
		}
		//�E�F�[�u���G�t�F�N�g�X�V
		for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
			boss1WaveDeathEffect[i].Updata();
			if (boss1WaveDeathEffect[i].count > boss1WaveDeathEffect[i].countMax) {
				//�E�F�[�u���G�t�F�N�g�폜
				boss1WaveDeathEffect.erase(boss1WaveDeathEffect.begin() + i);
			}
		}
		//���S����
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//���S�����i�A�j���[�V�����X�g�b�v�j
		deathCount++;
		if (deathCount > 60) {
			deathCount = 60;
		}
	}


	
	//�X�e�[�W�ԍ�����
	gameSystem->stageNum = 1;
	//BOSS1�̏o�����̃J�����̑���
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 6;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//����BGM���~�߂�
	}
	else {

		//�o�����A�j���[�V����
		if (gameSystem->cameraBossCount == 20) {
			animeNum = 3;
		}
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}
	//�{�X���S���Ɏ��̖ʂֈړ�����
	if (animeNum == 0) {
		//�����Ă���
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//���S�ɏ��������{�X�̏������~����
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss1Exist = false;//���S�ȃ{�X�̒�~
				enemySystem->gameCount = 135 * 60;//�Q�[���J�E���g��Stage1���[�v����E�o������

				//�c���G�l�~�[�̔j��					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				//�X�e�[�W�ԍ���ύX
				gameSystem->stageNum = 2;
				gameSystem->cameraBossCount = 0;
				gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//����BGM���~�߂�
			}
		}
	}
}
void Boss1::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss1->RenderShadowMap(&md);

	//�{�X�̃E�F�[�u�U���`��
	for (int i = 0; i < boss1Wave.size(); i++)
	{
		gm->gm->DangerBlock->RenderShadowMap(&boss1Wave[i].md);
	}

	//�{�X�̃E�F�[�u���G�t�F�N�g
	for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
		for (int h = 0; h < boss1WaveDeathEffect[i].angle.size(); h++) {
			MODEL_DATA md;
			Pos2 tmp = GetCirclePoint(boss1WaveDeathEffect[i].md.pos.X, boss1WaveDeathEffect[i].md.pos.Z, boss1WaveDeathEffect[i].r, boss1WaveDeathEffect[i].angle[h]);
			md.pos.X = tmp.p[0];
			md.pos.Z = tmp.p[1];
			md.pos.Y = boss1WaveDeathEffect[i].md.pos.Y - boss1WaveDeathEffect[i].count * 0.05f;
			md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			float size = (float)GetRand(100, 200) / 1000.0f;
			md.scale = Vector3(size, size, size);

			gm->gm->DangerBlock->RenderShadowMap(&md);
		}
	}
}
void Boss1::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	//�댯�ȏꏊ�̕`��
	MODEL_DATA _md;
	switch (animeNum) {
	case 1://FRONT_ATTACK
		_md.scale = Vector3(0.35, 0.35, 0.35);
		for (int x = 2; x <= 6; x++) {

			for (int y = 0; y <= 4; y++) {
				_md.pos.X = (x - 4) * 0.66;
				_md.pos.Z = (y - 4) * -0.66;

				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 4://TACKLE_ATTACK
		_md.scale = Vector3(0.35, 0.35, 0.35);
		for (int x = 2; x <= 6; x++) {
			for (int y = 0; y <= 8; y++) {
				_md.pos.X = (x - 4) * 0.66;
				_md.pos.Z = (y - 4) * -0.66;
				_md.pos.Y = 0.001;

				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 5://WAVE_ATTACK

		break;
	}

	//�{�X�̕`��
	if (animeNum == 0 && deathCount >= 60) {
		gm->gm->Boss1->Animation(animeNum, 0);//���S���[�V�����̃X�g�b�v
	}
	else {
		if(gameSystem->pause==false)
			gm->gm->Boss1->Animation(animeNum, 1);
		else
			gm->gm->Boss1->Animation(animeNum, 0);
	}

	gm->gm->Boss1->RenderShadowModel(&md);

	//�{�X�̃E�F�[�u�U���`��
	for (int i = 0; i < boss1Wave.size(); i++)
	{
		gm->gm->DangerBlock->RenderShadowModel(&boss1Wave[i].md);
	}
	//�{�X�̃E�F�[�u���G�t�F�N�g
	for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
		for (int h = 0; h < boss1WaveDeathEffect[i].angle.size(); h++) {
			MODEL_DATA md;
			Pos2 tmp = GetCirclePoint(boss1WaveDeathEffect[i].md.pos.X, boss1WaveDeathEffect[i].md.pos.Z, boss1WaveDeathEffect[i].r, boss1WaveDeathEffect[i].angle[h]);
			md.pos.X = tmp.p[0];
			md.pos.Z = tmp.p[1];
			md.pos.Y = boss1WaveDeathEffect[i].md.pos.Y - boss1WaveDeathEffect[i].count * 0.05f;
			md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			float size = (float)GetRand(100, 200) / 1000.0f;
			md.scale = Vector3(size, size, size);

			gm->gm->DangerBlock->RenderShadowModel(&md);
		}
	}
}



void Boss2::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 1, 4);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 5;
	HP_MAX = 3000;//����HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//�U���Ԋu
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss2::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//��莞�Ԃ����Ƀv���C���[�̈ʒu�����čs�������߂�
						//��莞�Ԃ����Ƀv���C���[�̈ʒu�����čs�������߂�
		if (animeNum == 1||animeNum==6) {
			attackCount++;
		}
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;

			//�p�^�[���Ȃ��A���S�Ƀ����_����
			int parsent = GetRand(1, 100);
			if (parsent < 40) {
				animeNum = 2;
			}
			else if (parsent < 70) {
				animeNum = 3;
			}
			else {
				animeNum = 4;
			}

		}

		switch (animeNum) {
		case 0://���S
			break;
		case 1://�ҋ@
			break;
		case 2://�z�[�~���O
			animeCount++;
			if (animeCount == 30) {
				for (int h = 0; h < 2; h++) {
					for (int i = 0; i < 7; i++) {
						float dist = 0.66;
						Pos2 tmpPos;

						if (i - 3 == 0) {
							continue;
						}

						tmpPos.p[0] = dist * (i - 3);


						tmpPos.p[1] = 4;

						float speed = 0.09;
						if (i < 4)
							enemySystem->AddEnemy(100, 0.005, tmpPos, Homing_Bezier, speed, 0.001, 0, i * 15+(h*7*15));
						else
							enemySystem->AddEnemy(100, 0.005, tmpPos, Homing_Bezier, speed, 0.001, 1, i * 15 + (h * 7 * 15));
					}
				}
			}
			for (int i = 0; i < 14; i++) {
				if (animeCount == 30 + i * 15) {
					gm->gs->Boss2Homing.Play(false);
				}		
			}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 3://���[�U�[�c
			animeCount++;

			if (animeCount % 15 == 0) {
				gm->gs->Boss2Laser.Play(false);
				int rand[4] = { 100,100,100,100 };
				for (int i = 0; i < 4; i++) {
					float dist = 0.66;
					Pos2 tmpPos;

					int rand2 = NULL;
					do {
						rand2 = (GetRand(0, 8) - 4);
					} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

					tmpPos.p[0] = dist * rand2;
					tmpPos.p[1] = 4;

					enemySystem->AddEnemy(100, 0.05, tmpPos, DownMove);

					rand[i] = rand2;
				}
			}

			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 4://���[�U�[��
			animeCount++;
			if (animeCount % 15 == 0) {
				gm->gs->Boss2Laser.Play(false);
				int rand[4] = { 100,100,100,100 };
				for (int i = 0; i < 3; i++) {
					float dist = 0.66;
					Pos2 tmpPos;

					int rand2 = NULL;
					do {
						rand2 = (GetRand(0, 8) - 4);
					} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

					tmpPos.p[0] = 4;
					tmpPos.p[1] = dist * rand2;

					enemySystem->AddEnemy(100, 0.04, tmpPos, LeftMove);
					tmpPos.p[0] = -4;
					enemySystem->AddEnemy(100, 0.04, tmpPos, RightMove);

					rand[i] = rand2;
				}
			}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 5://�o��
			animeCount++;
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 6://�_���[�W���󂯂���
			animeCount++;
			if (animeCount >= 30) {
				animeCount = 0;
				animeNum = 1;
			}
		}

		//�v���C���[�U���G�t�F�N�g�Ƃ̓����蔻��
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;
					point.p[1] = md.pos.Y-1;
					point.p[2] = md.pos.Z-1;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);

						//���炢���[�V����
						if (animeNum == 1) {

							animeNum = 6;
						}
						break;

					}
				}
			}
		}
		//���G����
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//���S����
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//���S�����i�A�j���[�V�����X�g�b�v�j
		deathCount++;
		if (deathCount > 70) {
			deathCount = 70;
		}
	}

	//�X�e�[�W�ԍ�����
	gameSystem->stageNum = 3;
	//BOSS1�̏o�����̃J�����̑���
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 5;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//����BGM���~�߂�
	}
	else {

		//�o�����A�j���[�V����
		if (gameSystem->cameraBossCount == 20) {
			animeNum = 1;
		}
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}
	//�{�X���S���Ɏ��̖ʂֈړ�����
	if (animeNum == 0) {
		//�����Ă���
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//���S�ɏ��������{�X�̏������~����
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss2Exist = false;//���S�ȃ{�X�̒�~
				enemySystem->gameCount = 295 * 60;//�Q�[���J�E���g��Stage1���[�v����E�o������

				//�c���G�l�~�[�̔j��					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				//�X�e�[�W�ԍ���ύX
				gameSystem->stageNum = 4;
				gameSystem->cameraBossCount = 0;
				gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//����BGM���~�߂�
			}
		}
	}
}
void Boss2::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss2->RenderShadowMap(&md);
}
void Boss2::Draw() {
	GameSystem* gameSystem = GameSystem::GetInstance();
	GameManager* gm = GameManager::GetInstance();
	if (animeNum == 0 && deathCount >= 69) {
		gm->gm->Boss2->Animation(animeNum, 0);//���S���[�V�����̃X�g�b�v
	}
	else {
		if(gameSystem->pause==false)
			gm->gm->Boss2->Animation(animeNum, 1);
		else
			gm->gm->Boss2->Animation(animeNum, 0);
	}
	gm->gm->Boss2->RenderShadowModel(&md);
}

void Boss3::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 0, 5);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 6;
	HP_MAX = 4000;//����HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//�U���Ԋu
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss3::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//��莞�Ԃ����Ƀv���C���[�̈ʒu�����čs�������߂�
		

		if (animeNum == 1||animeNum==7) {
			attackCount++;
		}
		//�U���ɂ͂���
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;

			//�E���R�}�X���Ȃ����ƉE��U�艺�낵
			if (player->x > 5) {
				animeNum = 3;
			}
			//�����R�}�X���Ȃ����ƍ���U�艺�낵
			else if (player->x < 3) {
				animeNum = 2;
			}
			//���S�R�}�X���ƃ��[�U�[�U��
			else if (3 <= player->x && player->x <= 5) {
				animeNum = 4;
			}
			//�����_���ŃT���_�[
			if (GetRand(1, 100) <= 15) {
				animeNum = 5;
			}
		}

		switch (animeNum) {
		case 0://���S
			break;
		case 1://�ҋ@
			break;
		case 2://�����U��
			//�v���C���[�ւ̃_���[�W����
			if (animeCount == 45) {
				gm->gs->Boss3Attack.Play(false);
				if (player->x <= 3 && player->damageCount == 0) {
					player->damage = true;
				}
			}


			animeCount++;
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 3://�E���U��

			//�v���C���[�ւ̃_���[�W����
			if (animeCount == 45) {
				gm->gs->Boss3Attack.Play(false);
				if (player->x >= 5 && player->damageCount == 0) {
					player->damage = true;
				}
			}
			animeCount++;

			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 4://���[�U�[
			animeCount++;
			if (animeCount == 60) {
				gm->gs->Boss3Laser.Play(false);
				for (int i = 0; i < 10; i++) {
					for (int h = 0; h < 8; h++) {
						Pos2 tmp = GetCirclePoint(0, 1, 1, h * 45);

						Enemy ene(100, 0.4, Vector3(tmp.p[0],tmp.p[1],3), DownMove);
						ene.startFrame = i * 8;
						enemySystem->AddEnemy(ene);
					}
				}
			}
			if (animeCount >= 60&& animeCount <= 80) {
				if (2 <= player->x && player->x <= 6&&player->damageCount==0) {
					player->damage = true;
				}		
			}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 5://�T���_�[
			animeCount++;
			if (animeCount == 30) {
				gm->gs->Boss3Thunder.Play(false);
				for (int i = 0; i < 10; i++) {
					for (int _x = -2; _x <= 2; _x++) {
						float rand = (GetRand(0, 10)-5)*0.1f;
						for (int _y = -2; _y <= 2; _y++) {
							
							float dist = 0.66;
							
							Enemy ene(100, 0.2, Vector3(_x * dist, -1+rand, _y * dist), AntiGravity);
							ene.startFrame = i * 10;
							ene.effectDataSize = 2;
							enemySystem->AddEnemy(ene);
						}
					}
				}
			}

			//if (animeCount == 60) {
			//	if (2 <= player->x && player->x <= 6 && 2 <= player->y && player->y <= 6) {
			//		player->damage = true;
			//	}
			//}
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 6://�o��
			animeCount++;
			//�A�j���[�V������߂�
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 7://�_���[�W���󂯂���
			animeCount++;
			if (animeCount >= 30) {
				animeCount = 0;
				animeNum = 1;
			}
		}

		//�v���C���[�U���G�t�F�N�g�Ƃ̓����蔻��
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;//����𒲐��ł���
					point.p[1] = md.pos.Y;
					point.p[2] = md.pos.Z-2;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);

						//���炢���[�V����
						if (animeNum == 1) {

							animeNum = 7;
						}
						break;

					}
				}
			}
		}
		//���G����
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//���S����
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//���S�����i�A�j���[�V�����X�g�b�v�j
		deathCount++;
		if (deathCount > 90) {
			deathCount = 90;
		}
	}

	//�X�e�[�W�ԍ�����
	gameSystem->stageNum = 5;
	//BOSS1�̏o�����̃J�����̑���
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 6;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//����BGM���~�߂�
	}
	else {

		//�o�����A�j���[�V����
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}

	//�{�X���S���Ɏ��̖ʂֈړ�����
	if (animeNum == 0) {
		//�����Ă���
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//���S�ɏ��������{�X�̏������~����
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss3Exist = false;//���S�ȃ{�X�̒�~
				enemySystem->gameCount = 1500 * 60;//�Q�[���J�E���g��Stage1���[�v����E�o������

				//�c���G�l�~�[�̔j��					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				gameSystem->cameraBossCount = 0;


			}
		}
	}
}
void Boss3::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss3->RenderShadowMap(&md);
}
void Boss3::Draw() {
	GameSystem* gameSystem = GameSystem::GetInstance();
	GameManager* gm = GameManager::GetInstance();

	MODEL_DATA _md;
	_md.scale = Vector3(0.35, 0.35, 0.35);
	switch (animeNum) {
	case 2://����U��
		for (int _x = -4; _x <= -1; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 3://�E��U��
		for (int _x = 1; _x <= 4; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 4://���[�U�[�U��
		for (int _x = -2; _x <= 2; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 5://�T���_�[
		for (int _x = -2; _x <= 2; _x++) {
			for (int _y = -2; _y <= 2; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	}
	

	
	if (animeNum == 0 && deathCount >= 89) {
		gm->gm->Boss3->Animation(animeNum, 0);//���S���[�V�����̃X�g�b�v
	}
	else {
		if (gameSystem->pause == false)
			gm->gm->Boss3->Animation(animeNum, 1);
		else
			gm->gm->Boss3->Animation(animeNum, 0);
	}
	gm->gm->Boss3->RenderShadowModel(&md);
}
void BossSystem::Reset() {
	boss1.Reset();
	boss2.Reset();
	boss3.Reset();
	boss1Exist = false;
	boss2Exist = false;
	boss3Exist = false;
}
void BossSystem::Updata() {
	if (boss1Exist) {
		boss1.Updata();
	}
	if (boss2Exist) {
		boss2.Updata();
	}
	if (boss3Exist) {
		boss3.Updata();
	}

}
void BossSystem::DrawShadow() {
	if (boss1Exist) {
		boss1.DrawShadow();
	}
	if (boss2Exist) {
		boss2.DrawShadow();
	}
	if (boss3Exist) {
		boss3.DrawShadow();
	}
	}
	
	
	
void BossSystem::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	DirectGraphics* dg=DirectGraphics::GetInstance();
	//�{�X��HP��UI�iHP�o�[�̕`��j �������኱����Ȃ̂łQD�`�悾�������ɏ������ނ��Ƃ�
	if (boss1Exist || boss2Exist || boss3Exist) {
		float bossHP = NULL;
		float bossHP_MAX = NULL;
		float bossDamageBlur = 0.0f;
		if (boss1Exist) {
			bossHP = boss1.HP;
			bossHP_MAX = boss1.HP_MAX;
			if (boss1.damageCount != 0 && boss1.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}


		}
		if (boss2Exist) {
			bossHP = boss2.HP;
			bossHP_MAX = boss2.HP_MAX;
			if (boss2.damageCount != 0 && boss2.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}
		}
		if (boss3Exist) {
			bossHP = boss3.HP;
			bossHP_MAX = boss3.HP_MAX;
			if (boss3.damageCount != 0 && boss3.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}
		}
		//�{�X�̔�e���ɗh�炷����
			//�萔�o�b�t�@���V�F�[�_�[�ɐݒ�
		dg->GetConstantBufferData()->multiplyColor.z = 0;
		dg->GetConstantBufferData()->multiplyColor.w = 0;
		dg->GetConstantBufferData()->multiplyColor.y = 0;
		dg->GetConstantBufferData()->multiplyColor.x = ((bossHP_MAX / 2) / bossHP);
		if ((bossHP_MAX / 2) > bossHP) {
			dg->GetConstantBufferData()->multiplyColor.y = -((bossHP_MAX / 2) - bossHP)/ (bossHP_MAX / 2);
		}
		
		

		dg->GetContext()->UpdateSubresource(dg->GetConstantBuffer(), 0, NULL, dg->GetConstantBufferData(), 0, 0);
		ID3D11Buffer* constant_buffer = dg->GetConstantBuffer();
		//dg->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		//dg->GetContext()->GSSetConstantBuffers(0, 1, &constant_buffer);
		dg->GetContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		gm->gi->hpBarBase.Draw(960 + bossDamageBlur, 150 + bossDamageBlur, 0, 1, 1 * ((float)gameSystem->cameraBossCount / 120.0f * 2));
		int tmp = 1920 - (1.0f - bossHP / bossHP_MAX) * 1920.0f;
		gm->gi->hpBar.DrawEX(960 + bossDamageBlur, 150 + bossDamageBlur, 0, 1, 1, 0.7 * ((float)gameSystem->cameraBossCount / 120.0f * 2), 1, false, false, true, RECT{ 0,0,tmp,225 });
	}

	//�e�{�X�̂RD�`��
	if (boss1Exist) {
		boss1.Draw();
	}
	if (boss2Exist) {
		boss2.Draw();
	}
	if (boss3Exist) {
		boss3.Draw();
	}
}
