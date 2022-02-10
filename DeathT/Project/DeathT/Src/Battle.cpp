#include "GameManager.h"
#include "EnemySystem.h"
#include "Player.h"
#include "BossSystem.h"
#include "GameSystem.h"
namespace BattleVariable {

	Player* player;
	EnemySystem* enemySystem;
	BossSystem* bossSystem;
	GameSystem* gameSystem;

	

	
	int backCount = 0;//�w�i�X�N���[���p
	int backCountSpeed = 5;

	float backAlpha = 1.0f;
	int stageNumOld = 0;
	bool backChange = false;

	int clearCount = 0;//�N���A��ʂɍs���܂ł̂���

	int scoreTime = 0;//�N���A����

}
using namespace BattleVariable;

void GameManager::BattleLoad(int _stageNum) {


	player = Player::GetInstance();
	enemySystem = EnemySystem::GetInstance();
	bossSystem = BossSystem::GetInstance();
	gameSystem = GameSystem::GetInstance();

	backCount = 0;
	backCountSpeed = 5;
	clearCount = 0;
	scoreTime = 0;


	backAlpha = 1.0f;
	stageNumOld = 0;
	backChange = false;
	//0~5
	//0 start1
	//1 Boss1
	//2 start2
	//3 Boss2
	//4 start3
	//5 Boss3
	_stageNum = 0;

	gameSystem->Reset(_stageNum);
	player->Reset();
	enemySystem->Reset();
	bossSystem->Reset();
	

}

void GameManager::BattleUpdata() {
	
	if (KEY->key[DIK_ESCAPE] == 1) {
		gameSystem->pause = !gameSystem->pause;
	}
	if (gameSystem->pause == false) {
		if (scoreTime < 60 * 60 * 60) {
			scoreTime++;//����͂P����
		}
		backCount += backCountSpeed;

		//�w�i�ϊ��p
		if (stageNumOld != gameSystem->stageNum / 2) {
			backChange = true;
			backAlpha = 1.0f;
		}
		if (backChange == true) {
			backAlpha -= 0.01f;
			if (backAlpha <= 0) {
				backAlpha = 0;
				backChange = false;
			}
		}
		stageNumOld = gameSystem->stageNum / 2;

		gs->BGM[5 + gameSystem->stageNum].Play();//BGM���Đ�


		player->Updata();//�v���C���[���C������ Updata�Ɠ���

		enemySystem->Updata();//�U�R�I�V�X�e���̃��C������

		bossSystem->Updata();

		if (bossSystem->boss1Exist == false &&
			bossSystem->boss2Exist == false &&
			bossSystem->boss3Exist == false &&
			bossSystem->boss3.HP <= 0
			) {
			clearCount++;
			if (clearCount >= 150) {
				Battle_ClearSetScore(scoreTime, enemySystem->enemyDeleteNum);

				gs->BGM[scene + gameSystem->stageNum].Stop();

				scene = BATTLE_CLEAR;

				sceneOld = BATTLE;
			}
		}

	}

}

void GameManager::BattleDraw() {
	//�_���[�W�󂯂����ɃJ�������u��������
	float damageBlur = 0.0f;
	if (player->hpCount != -1) {
		damageBlur = (float)GetRand(1, 10) / 10.0f - 0.5;
	}
	DirectGraphics::GetInstance()->SetLight(Vector3(0, 15, 5), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
	//�e�̕`��/////////////////////////////////////////////////
	//�X�e�[�W�̕`��
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			MODEL_DATA md;
			md.scale = Vector3(0.35, 0.35, 0.35);
			md.pos.X = (x - 4) * 0.66;
			md.pos.Z = (y - 4) * 0.66;

			gm->playerAttackEffect_Red->RenderShadowMap(&md);
		}
	}
	enemySystem->DrawShadow();
	bossSystem->DrawShadow();
	player->DrawShadow();
	////////////////////////////////////////////////////////

	DirectGraphics::GetInstance()->SetCamera(Vector3(0 + damageBlur, 6 + gameSystem->cameraDistY, -3 * 2.5 + damageBlur - gameSystem->cameraDistY), Vector3(0, 1, 0 - gameSystem->cameraDistY), Vector3(0, 1, 0));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);
	//�{�`��i���ۂ̕`��j

	//�Q�[���̔w�i(����)
	int gameBackX = 960 - backCount % 1920;
	int gameBackY = 540 + backCount % 1080;

	switch (gameSystem->stageNum / 2) {
	case 0:
		gi->battle[0].Draw(gameBackX, gameBackY - 1080, 0, 1, backAlpha);
		gi->battle[0].Draw(gameBackX, gameBackY, 0, 1, backAlpha);
		gi->battle[0].Draw(gameBackX + 1920, gameBackY, 0, 1, backAlpha);
		gi->battle[0].Draw(gameBackX + 1920, gameBackY - 1080, 0, 1, backAlpha);
		break;
	case 1:
	case 2:
		gi->battle[gameSystem->stageNum / 2 - 1].Draw(gameBackX, gameBackY - 1080, 0, 1, backAlpha);
		gi->battle[gameSystem->stageNum / 2 - 1].Draw(gameBackX, gameBackY, 0, 1, backAlpha);
		gi->battle[gameSystem->stageNum / 2 - 1].Draw(gameBackX + 1920, gameBackY, 0, 1, backAlpha);
		gi->battle[gameSystem->stageNum / 2 - 1].Draw(gameBackX + 1920, gameBackY - 1080, 0, 1, backAlpha);

		gi->battle[gameSystem->stageNum / 2].Draw(gameBackX, gameBackY - 1080, 0, 1, 1 - backAlpha);
		gi->battle[gameSystem->stageNum / 2].Draw(gameBackX, gameBackY, 0, 1, 1 - backAlpha);
		gi->battle[gameSystem->stageNum / 2].Draw(gameBackX + 1920, gameBackY, 0, 1, 1 - backAlpha);
		gi->battle[gameSystem->stageNum / 2].Draw(gameBackX + 1920, gameBackY - 1080, 0, 1, 1 - backAlpha);
		break;

	}


	//�X�e�[�W�̕`��
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			MODEL_DATA md;
			md.scale = Vector3(0.35, 0.35, 0.35);
			md.pos.X = (x - 4) * 0.66;
			md.pos.Z = (y - 4) * 0.66;
			//md.multiplyColor.w = 0.5;
			gm->playerAttackEffect_Red->RenderShadowModel(&md);
		}
	}

	enemySystem->Draw();

	player->Draw();

	bossSystem->Draw();


	//�v���C���[HP��UI �@���̃A�C�R��
	for (int i = 0; i < player->HP; i++) {
		gi->barIcon.Draw(100 + i * 150, 980, 0);
	}
	if (player->hpCount != -1) {
		gi->barIcon.Draw(100 + (player->HP) * 150, 980, player->hpCount * 6, 1.0f - ((float)player->hpCount / (float)player->hpCountMax));
	}
	//BOM��UI
	for (int i = 0; i < player->BOM; i++) {
		gi->bom.Draw(100 + i * 150, 850, 0);
	}
	if (player->bomCount != -1) {
		gi->bom.Draw(100 + (player->BOM) * 150, 850, player->bomCount * 6, 1.0f - ((float)player->bomCount / (float)player->bomCountMax));
	}

	//�|�[�Y��ʂ̏����i�V�X�e�����g�ݍ��܂�Ă��邽�ߏ�����������ɋL�q�j
	if (gameSystem->pause) {
		gi->pause.Draw(960, 540);
		
		//�I�����m��
		if (gameSystem->pauseSelectConfirm) {
			gi->pause_SelectConfirm.Draw(960, 600);
			//�Z���N�g��ʂɖ߂�
			if (gi->pause_SelectYes.Hit(MOUSE->x, MOUSE->y)) {
				gi->pause_SelectYes.Draw(740, 890, 0, 1.2);
	
				if (MOUSE->left == 1) {
					gs->Enter.Play(false);
					gs->BGM[scene + gameSystem->stageNum].Stop();
					scene = SELECT;
					sceneOld = BATTLE;
				}
			}
			else {
				gi->pause_SelectYes.Draw(740, 890);
			}
			//�|�[�Y�ɖ߂�
			if (gi->pause_SelectNo.Hit(MOUSE->x, MOUSE->y)) {

				gi->pause_SelectNo.Draw(1180, 890, 0, 1.2);
				if (MOUSE->left == 1) {
					gameSystem->pauseSelectConfirm = false;
					MOUSE->left = 2;
				}
			}
			else {
				gi->pause_SelectNo.Draw(1180, 890);
			}
		}
		else {
			if (gi->pause_Select.Hit(MOUSE->x, MOUSE->y)) {
			
				gi->pause_Select.Draw(960, 720, 0, 1.2);
				if (MOUSE->left == 1) {
					gameSystem->pauseSelectConfirm = true;
				}
			}
			else {
				gi->pause_Select.Draw(960, 720);
			}
			if (gi->pause_Back.Hit(MOUSE->x, MOUSE->y)) {
				
				gi->pause_Back.Draw(960, 960, 0, 1.2);
				if (MOUSE->left == 1) {
					gameSystem->pause = false;
				}
			}
			else {
				gi->pause_Back.Draw(960, 960);
			}
		}

	}

	//�u���b�N�t�F�[�h
		//�N���A�G�t�F�N�g
	if (clearCount) {
		gi->feedWhite.Draw(960, 540, 0, 1, (float)clearCount / 100.0f);
	}
	if (player->death) {
		gi->feedBlack.Draw(960, 540, 0, 1, (float)player->deathCount / 100.0f);
	}
}
