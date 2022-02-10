#include "Player.h"
#include "Player12.h"

void Stage::Reset(int stageNum) {
	playerWin[0] = 3;
	playerWin[1] = 3;
	
	
}
void PlayerBase::ResetBase() {

	invincibleCount = 1;

	y = 4;


	speed = 6;

	moveNum = 0;

	death = false;

	for (int i = 0; i < playerAttackEffect.size(); i++) {
		playerAttackEffect.pop_back();
	}


	md.degree = Vector3(0.0f, 180.0f, 180.0f);
	md.scale = Vector3(0.3, 0.3, 0.3);
	md.axis = Vector3(1, 1, 1);
	md.axisAngle = 0;
	md.axisR = Vector3(0, 0, 0);
	md.multiplyColor.x = 1;
	md.multiplyColor.y = 1;
	md.multiplyColor.z = 1;
	md.multiplyColor.w = 1;

	
	degree = md.degree;
	scale = md.scale;
}
void PlayerBase::UpdataBase() {
	GameManager* gm = GameManager::GetInstance();
	PlayerBase* enemyPlayer;
	Stage* stage = Stage::GetInstance();

	if (playerNum == 0) {
		enemyPlayer = Player2::GetInstance();
	}
	else {
		enemyPlayer = Player1::GetInstance();
	}

	//�v���C���[�̖��G����
	if (invincibleCount > 0 && invincibleCount < invincibleCountMax) {
		invincibleCount++;
	}
	else {
		invincibleCount=0;
	}

	

	//���ʂƂ��̏���

	//���S�G�t�F�N�g�X�V
	for (int i = 0; i < playerDeathEffect.size(); i++) {
		playerDeathEffect[i].Updata();
		if (playerDeathEffect[i].count > playerDeathEffect[i].countMax) {
			//���S�G�t�F�N�g�폜
			playerDeathEffect.erase(playerDeathEffect.begin() + i);
		}
	}

	float r = GetHypotenuse(0.325, 0.325);
	int moveDistF = 2;

	//�v���C���[�U���G�t�F�N�g�Ƃ̓����蔻��
	for (int i = 0; i < enemyPlayer->playerAttackEffect.size(); i++) {
		for (int h = 0; h < 3; h++) {
			Pos3 center;
			center.p[0] = enemyPlayer->playerAttackEffect[i].md[h].pos.X;
			center.p[1] = enemyPlayer->playerAttackEffect[i].md[h].pos.Y;
			center.p[2] = enemyPlayer->playerAttackEffect[i].md[h].pos.Z;
			Pos3 point;
			point.p[0] = md.pos.X;
			point.p[1] = md.pos.Y - 0.3;
			point.p[2] = md.pos.Z;
			Pos3 size;
			size.p[0] = 0.66;
			size.p[1] = 0.66;
			size.p[2] = 0.66;

			if (HitRect3D(center, size, point)&&invincibleCount==0) {
				if (death == false) {
					//���S�G�t�F�N�g�̒ǉ�
					PlayerDeathEffect pb;
					pb.md = md;
					playerDeathEffect.push_back(pb);
					gm->gs->Bom.Play(false);
					invincibleCount = 1;
					stage->playerWin[playerNum]--;
				}
				//���S
				death = true;
				Reset();
			}
		}
	}

	//�v���C���[���앪��p
	int keySave[4];
	if (playerNum == 1) {
		keySave[0] = gm->KEY->key[DIK_W];
		keySave[1] = gm->KEY->key[DIK_A];
		keySave[2] = gm->KEY->key[DIK_S];
		keySave[3] = gm->KEY->key[DIK_D];

		gm->KEY->key[DIK_W] = gm->KEY->key[DIK_UP];
		gm->KEY->key[DIK_A] = gm->KEY->key[DIK_LEFT];
		gm->KEY->key[DIK_S] = gm->KEY->key[DIK_DOWN];
		gm->KEY->key[DIK_D] = gm->KEY->key[DIK_RIGHT];

	}
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

	//�U���G�t�F�N�g�X�V
	for (int i = 0; i < playerAttackEffect.size(); i++) {
		playerAttackEffect[i].Updata();
	}
	for (int i = 0; i < playerAttackEffect.size(); i++) {

		if (playerAttackEffect[i].count > playerAttackEffect[i].countMax) {

			playerAttackEffect.erase(playerAttackEffect.begin());
		}
	}

	//�L�[��߂�
	if (playerNum == 1) {

		gm->KEY->key[DIK_W] = keySave[0];
		gm->KEY->key[DIK_A] = keySave[1];
		gm->KEY->key[DIK_S] = keySave[2];
		gm->KEY->key[DIK_D] = keySave[3];

	}
}
void PlayerBase::DrawShadowBase() {
	GameManager* gm = GameManager::GetInstance();
	//�e�`��̍ۂɃ��f���͊֌W�Ȃ��̂œK���Ȃ��̂��g�p
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

void PlayerBase::DrawBase() {
	GameManager* gm = GameManager::GetInstance();

	//if (death)
	//	return;

	//�v���C���[�̕\��
	if (md.scale.Y > 0.01) {
		if (invincibleCount > 0) {
			md.multiplyColor.w = 0.5;
		}

		if(playerNum==0)
			gm->gm->Player->RenderShadowModel(&md);
		else
			gm->gm->Player2->RenderShadowModel(&md);

		md.multiplyColor.w = 1;
	}

	//�v���C���[�U���G�t�F�N�g�̕`��
	for (int i = 0; i < playerAttackEffect.size(); i++) {
		for (int h = 0; h < 3; h++) {
			if ((playerAttackEffect[i].md[h].pos.X > -0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.X < 0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z>-0.66 * 4.5 &&
				playerAttackEffect[i].md[h].pos.Z < 0.66 * 4.5)
				) {
				if (playerNum == 0)
					gm->gm->playerAttackEffect_RedRed->RenderShadowModel(&playerAttackEffect[i].md[h]);
				else
					gm->gm->playerAttackEffect_Blue->RenderShadowModel(&playerAttackEffect[i].md[h]);
			}
		}
	}

	//���S�G�t�F�N�g �d���Ă����؂�
	for (int i = 0; i < playerDeathEffect.size(); i++) {
		MODEL_DATA _md;
		_md.pos.Y = playerDeathEffect[i].md.pos.Y;

		float size = 0.3;
		

		for (int h = 0; h < playerDeathEffect[i].angle.size(); h++) {

			_md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			size = (float)GetRand(100, 200) / 500.0f;
			_md.scale = Vector3(size, size, size);


			Pos2 tmp = GetCirclePoint(playerDeathEffect[i].md.pos.X, playerDeathEffect[i].md.pos.Z, playerDeathEffect[i].r, playerDeathEffect[i].angle[h]);
			_md.pos.X = tmp.p[0];
			_md.pos.Z = tmp.p[1];

			if (playerNum == 0)
				gm->gm->playerAttackEffect_RedRed->RenderShadowModel(&_md);
			else
				gm->gm->playerAttackEffect_Blue->RenderShadowModel(&_md);
		}
	}

}
void Player1::Reset() {
	ResetBase();
	//�X�e�[�W��ł̃}�X�ڔԍ�
	x = 0;//����	
	md.pos = Vector3(0.0f - 4 * 0.66f, 0.65f, 0.0f);
	pos = md.pos;
	playerNum = 0;
}
void Player2::Reset() {
	ResetBase();
	//�X�e�[�W��ł̃}�X�ڔԍ�
	x = 8;//����	
	md.pos = Vector3(0.0f + 4 * 0.66f, 0.65f, 0.0f);
	pos = md.pos;
	playerNum = 1;
}
void Player1::Updata() {
	UpdataBase();
}
void Player2::Updata() {
	UpdataBase();
}
void Player1::DrawShadow() {
	DrawShadowBase();
}
void Player2::DrawShadow() {
	DrawShadowBase();
}
void Player1::Draw() {
	DrawBase();
}
void Player2::Draw() {
	DrawBase();
}
