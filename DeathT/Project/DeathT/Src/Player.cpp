#include "Player.h"
#include "EnemySystem.h"
void Player::Reset() {

	 GameManager* gm=GameManager::GetInstance();

	//�X�e�[�W��ł̃}�X�ڔԍ�
	x = 4;//����
	y = 4;
	height = 0;//����

	HP = 5;
	BOM = 3;
	hpCount = -1;
	bomCount = -1;

	power = 100;
	speed = 6;

	moveNum = 0;

	damage = false;
	damageCount = 0;//���G���ԂŎg��
	death = false;
	deathCount = 0;
	for (int i = 0; i < playerAttackEffect.size(); i++) {
		playerAttackEffect.pop_back();
	}

	md.pos = Vector3(0.0f, 0.65f, 0.0f);
	md.degree = Vector3(0.0f, 180.0f, 180.0f);
	md.scale = Vector3(0.3, 0.3, 0.3);
	md.axis = Vector3(1, 1, 1);
	md.axisAngle = 0;
	md.axisR = Vector3(0, 0, 0);
	md.multiplyColor.x = 1;
	md.multiplyColor.y = 1;
	md.multiplyColor.z = 1;
	md.multiplyColor.w = 1;

	pos = md.pos;
	degree = md.degree;
	scale = md.scale;
}

void Player::Updata() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();

	EnemySystem* enemySystem = EnemySystem::GetInstance();

	if (HP <= 0) {
		float tmpSpeed = 0.01f;
		if (md.scale.X > 0) {
			md.scale.X -= tmpSpeed;
			md.scale.Y -= tmpSpeed;
			md.scale.Z -= tmpSpeed;
			md.degree.Y += 10;
		}
		else {
			death = true;
		}
	}
	if (hpCount != -1) {
		hpCount++;
		if (hpCount >= hpCountMax) {
			hpCount = -1;
		}
	}
	if (bomCount != -1) {
		bomCount++;
		if (bomCount >= bomCountMax) {
			bomCount = -1;
		}
	}
	float r = GetHypotenuse(0.325, 0.325);
	int moveDistF = 2;
	if (death == false) {
		if (moveNum == 0) {
			if ((gm->KEY->key[DIK_A] == 1 || gm->KEY->key[DIK_A] % moveDistF == 1) && x > 0) {
				moveNum = DIK_A;
				md.axis = Vector3(0, 0, 1);//��]���ݒ� Z
				md.axisR = Vector3(r, 0, 0);//���a�ݒ�

				md.axisAngle = 45;

				md.degree.Z += 45;

				md.pos.X = pos.X - 0.33;

				md.pos.Y = pos.Y - 0.33;

			}
			else if ((gm->KEY->key[DIK_D] == 1 || gm->KEY->key[DIK_D] % moveDistF == 1) && x < 8) {
				moveNum = DIK_D;
				md.axis = Vector3(0, 0, 1);//��]���ݒ� Z
				md.axisR = Vector3(r, 0, 0);//���a�ݒ�

				md.axisAngle = 135;
				md.degree.Z += 135;

				md.pos.X = pos.X + 0.33;
				md.pos.Y = pos.Y - 0.33;

			}
			else if ((gm->KEY->key[DIK_W] == 1 || gm->KEY->key[DIK_W] % moveDistF == 1) && y > 0) {
				moveNum = DIK_W;

				md.axis = Vector3(1, 0, 0);//��]���ݒ� X
				md.axisR = Vector3(0, 0, r);//���a�ݒ�

				md.axisAngle = -135;

				md.degree.X -= 45;

				md.pos.Z = pos.Z + 0.33;

				md.pos.Y = pos.Y - 0.33;

			}
			else if ((gm->KEY->key[DIK_S] == 1 || gm->KEY->key[DIK_S] % moveDistF == 1) && y < 8) {
				moveNum = DIK_S;
				md.axis = Vector3(1, 0, 0);//��]���ݒ� Z
				md.axisR = Vector3(0, 0, r);//���a�ݒ�


				md.axisAngle = -45;
				md.degree.X -= 45;

				md.pos.Z = pos.Z - 0.33;

				md.pos.Y = pos.Y - 0.33;

			}
		}
		else {

			int dist = 0;
			float speedReal = 0.655;

			switch (moveNum) {
			case DIK_A:

				md.axisAngle += speed;
				if (md.axisAngle >= 135 + dist) {
					moveNum = 0;
					pos.X -= speedReal;
					//pos.Y += speedReal;

					md.pos = pos;

					md.degree.Z += -135 - dist;

					md.axisAngle = 0;
					md.axisR = Vector3(0, 0, 0);//���a�ݒ�

					x--;

					Pos2 tmpPos; tmpPos.p[0] = x; tmpPos.p[1] = y;
					PlayerAttackEffect pae(md, tmpPos, Left);
					playerAttackEffect.push_back(pae);

					gm->gs->MovePlayer.Play();
				}
				break;
			case DIK_D:

				md.axisAngle -= speed;
				if (md.axisAngle <= 45 - dist) {
					moveNum = 0;
					pos.X += speedReal;
					//	pos.Y += speedReal;

					md.pos = pos;
					md.degree.Z += -45 + dist;

					md.axisAngle = 0;
					md.axisR = Vector3(0, 0, 0);//���a�ݒ�

					x++;
					Pos2 tmpPos; tmpPos.p[0] = x; tmpPos.p[1] = y;
					PlayerAttackEffect pae(md, tmpPos, Right);
					playerAttackEffect.push_back(pae);

					gm->gs->MovePlayer.Play();
				}
				break;
			case DIK_W:

				md.axisAngle += speed;
				if (md.axisAngle >= -45 + dist) {
					moveNum = 0;
					pos.Z += speedReal;
					//	pos.Y += speedReal;

					md.pos = pos;
					md.degree.X += 45;

					md.axisAngle = 0;
					md.axisR = Vector3(0, 0, 0);//���a�ݒ�

					y--;
					Pos2 tmpPos; tmpPos.p[0] = x; tmpPos.p[1] = y;
					PlayerAttackEffect pae(md, tmpPos, Up);
					playerAttackEffect.push_back(pae);
					gm->gs->MovePlayer.Play();
				}
				break;
			case DIK_S:

				md.axisAngle -= speed;
				if (md.axisAngle <= -135 - dist) {
					moveNum = 0;
					pos.Z -= speedReal;
					//	pos.Y += speedReal;

					md.pos = pos;
					md.degree.X += 45;

					md.axisAngle = 0;
					md.axisR = Vector3(0, 0, 0);//���a�ݒ�


					y++;
					Pos2 tmpPos; tmpPos.p[0] = x; tmpPos.p[1] = y;
					PlayerAttackEffect pae(md, tmpPos, Down);
					playerAttackEffect.push_back(pae);
					gm->gs->MovePlayer.Play();
				}
				break;

			}
		}
	}

	for (int i = 0; i < playerAttackEffect.size(); i++) {
		playerAttackEffect[i].Updata();
	}
	for (int i = 0; i < playerAttackEffect.size(); i++) {

		if (playerAttackEffect[i].count > playerAttackEffect[i].countMax) {

			playerAttackEffect.erase(playerAttackEffect.begin());
		}
	}

	GameSound* gs = GameSound::GetInstance();
	//�{������
	if (gm->MOUSE->left == 1&& BOM>0) {
		//�{���G�t�F�N�g�̒ǉ�
		PlayerBomEffect pb;
		pb.md = md;
		playerBomEffect.push_back(pb);

		BOM--;
		bomCount = 0;
		damageCount = 1;
		gm->gs->Bom.Play(false);
		//�c���G�l�~�[�̔j��					
		for (int i = 0; i < enemySystem->enemy.size(); i++) {
			enemySystem->enemy[i].death = true;
		}
	}
	//�{���G�t�F�N�g�X�V
	for (int i = 0; i <playerBomEffect.size(); i++) {
		playerBomEffect[i].Updata();
		if (playerBomEffect[i].count > playerBomEffect[i].countMax) {
			//�{���G�t�F�N�g�폜
			playerBomEffect.erase(playerBomEffect.begin() + i);
		}
	}
	//�_���[�W����
	if (damage && damageCount == 0) {
		gs->PlayerDamage.Play(false);
		damage = false;
		HP--;
		hpCount = 0;
		damageCount = 1;
	}
	//���G����
	if (damageCount) {
		if (damageCount % 10 < 5) {
			md.multiplyColor.w = 0.5;
		}
		else {
			md.multiplyColor.x = 2.0;
			md.multiplyColor.y = 0.5;
			md.multiplyColor.z = 0.5;
			md.multiplyColor.w = 1.0;
		}
		damageCount++;
		if (damageCount > damageCountMax) {
			damageCount = 0;
		}
	}
	else {
		md.multiplyColor.x = 1.0;
		md.multiplyColor.y = 1.0;
		md.multiplyColor.z = 1.0;
		md.multiplyColor.w = 1.0;
	}

	//���S����
	if (death) {
		if (deathCount == 0) {
			gs->PlayerDead.Play(false);
		}
		deathCount++;
		if (deathCount >= 100) {

			gs->BGM[gm->scene + gameSystem->stageNum].Stop();
			gm->scene = SELECT;
		}
	}
}

void Player::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	//�v���C���[�̉e�`��
	if (md.scale.Y > 0.01) {
		gm->gm->Player->RenderShadowMap(&md);
	}
	//�v���C���[�U���G�t�F�N�g�̉e�`��
	for (int i = 0; i < playerAttackEffect.size(); i++) {
		for (int h = 0; h < 3; h++) {
			if ((playerAttackEffect[i].md[h].pos.X > -0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.X < 0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z>-0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z < 0.66 * 4.5)
				) {
				gm->gm->playerAttackEffect_Red->RenderShadowMap(&playerAttackEffect[i].md[h]);
			}
		}
	}
}
void Player::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	//�v���C���[�̕\��
	if (md.scale.Y > 0.01) {
		gm->gm->Player->RenderShadowModel(&md);
	}


	//�{���G�t�F�N�g
	for (int i = 0; i < playerBomEffect.size(); i++) {
		MODEL_DATA _md;
		_md.pos.Y = playerBomEffect[i].md.pos.Y;
		
		float size = 0.3;
		if (gameSystem->pause == false) {
			_md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			size = (float)GetRand(100, 200) / 500.0f;
		}
		else {
			md.degree = Vector3(i * 30, i * 30, i * 30);
		}

		_md.scale = Vector3(size, size, size);


		for (int h = 0; h < playerBomEffect[i].angle.size(); h++) {
			
			Pos2 tmp = GetCirclePoint(playerBomEffect[i].md.pos.X, playerBomEffect[i].md.pos.Z, playerBomEffect[i].r, playerBomEffect[i].angle[h]);
			_md.pos.X = tmp.p[0];
			_md.pos.Z = tmp.p[1];
		
			gm->gm->Player->RenderShadowModel(&_md);
		}
	}

	//�v���C���[�U���G�t�F�N�g�̕`��
	for (int i = 0; i < playerAttackEffect.size(); i++) {
		for (int h = 0; h < 3; h++) {
			if ((playerAttackEffect[i].md[h].pos.X > -0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.X < 0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z>-0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z < 0.66 * 4.5)
				) {

				gm->gm->playerAttackEffect_Red->RenderShadowModel(&playerAttackEffect[i].md[h]);
			}
		}
	}
}