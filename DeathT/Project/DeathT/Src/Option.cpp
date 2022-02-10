#include "GameManager.h"

namespace OptionVariable {
	int cursorNum = 0;
	int cursorPosX[3];

	//エフェクト系
	int zoomCount = 0;
	bool zoomBool = true;
	const int zoomCountMax = 10;
	float zoomSize = 1;
	const int zoomSpeed = 6;

	bool backBool = false;
}
using namespace OptionVariable;

void GameManager::OptionLoad() {
	cursorPosX[0] = 1375 + 450 * volumeMaster / 100;
	cursorPosX[1] = 1375 + 450 * volumeBGM / 100;
	cursorPosX[2] = 1375 + 450 * volumeSE / 100;

	zoomCount = 0;
	zoomSize = 1;

	backBool = false;
}
void GameManager::OptionUpdata() {

	if (cursorNum != 0) {
		//カーソルの移動処理
		cursorPosX[cursorNum - 1] = MOUSE->x / GetDefaultImageExtend();
		if (cursorPosX[cursorNum - 1] < 1375) {
			cursorPosX[cursorNum - 1] = 1375;
		}
		else if (cursorPosX[cursorNum - 1] > 1850) {
			cursorPosX[cursorNum - 1] = 1850;
		}
		//カーソル位置からボリュームを１～１００単位で計算
		switch (cursorNum) {
		case 1:
			volumeMaster = (cursorPosX[cursorNum - 1] - 1375) / 4.5;
			break;
		case 2:
			volumeBGM = (cursorPosX[cursorNum - 1] - 1375) / 4.5;
			break;
		case 3:
			volumeSE = (cursorPosX[cursorNum - 1] - 1375) / 4.5;
			break;
		}
		//ボリュームの実数値を計算
		bgmVolume = volumeMaster / 100.0f * volumeBGM / 100.0f;
		seVolume = volumeMaster / 100.0f * volumeSE / 100.0f;
	}
	else {
		gs->BGM[2].Volume(bgmVolume);
		gs->MoveCursor.Volume(seVolume);
	}
	//選択肢ズーム処理
	if (zoomBool) {
		zoomCount += zoomSpeed;
	}
	else {
		zoomCount -= zoomSpeed;
	}
	zoomSize = GetSinWave(zoomCount) + 0.5;
	if (zoomSize < 1) {
		zoomSize = 1;
	}
	if (zoomCount > zoomCountMax || zoomCount < 0) {
		zoomBool != zoomBool;
	}

	//前のシーンに戻る
	if (gi->optionBack.Hit(MOUSE->x, MOUSE->y) && MOUSE->left) {
		gs->BGM[scene].Stop();
		scene = sceneBack;
	
		//音量の設定
	//音量の設定
	//音量の設定
		for (int i = 0; i < 12; i++) {
			gs->BGM[i].Volume(bgmVolume);
		}
		gs->MovePlayer.Volume(seVolume);
		gs->SetBlock.Volume(seVolume);
		gs->Enter.Volume(seVolume);
		gs->Dead.Volume(seVolume);
		gs->MoveCursor.Volume(seVolume);


		gs->Attack.Volume(seVolume);
		gs->Boss1FrontAttack.Volume(seVolume);
		gs->Boss1Tackle.Volume(seVolume);
		gs->Boss1Avalanche.Volume(seVolume);
		gs->Boss2Laser.Volume(seVolume);
		gs->Boss2Homing.Volume(seVolume);
		gs->Boss3Attack.Volume(seVolume);
		gs->Boss3Laser.Volume(seVolume);
		gs->Boss3Thunder.Volume(seVolume);

		gs->EnemyDead.Volume(seVolume);
		gs->PlayerDamage.Volume(seVolume);
		gs->PlayerDead.Volume(seVolume);
		gs->ItemGet.Volume(seVolume);
		gs->Bom.Volume(seVolume);

	}

	cursorNum = 0;
}
void GameManager::OptionDraw() {
	gi->option.Draw(960, 540);
	gi->optionUI.Draw(960, 540);


	gi->optionCursor.Draw(cursorPosX[0], 480);
	if (gi->optionCursor.Hit(MOUSE->x, MOUSE->y)&&MOUSE->left) {
		cursorNum = 1;
		if(MOUSE->left==1)
			gs->MoveCursor.Play(false);
	}
	gi->optionCursor.Draw(cursorPosX[1], 720);
	if (gi->optionCursor.Hit(MOUSE->x, MOUSE->y) && MOUSE->left) {
		cursorNum = 2;
		if (MOUSE->left == 1)
		gs->MoveCursor.Play(false);
	}
	gi->optionCursor.Draw(cursorPosX[2], 960);
	if (gi->optionCursor.Hit(MOUSE->x, MOUSE->y) && MOUSE->left) {
		cursorNum = 3;
		if (MOUSE->left == 1)
		gs->MoveCursor.Play(false);
	}

	if (gi->optionBack.Hit(MOUSE->x, MOUSE->y)) {
		gi->optionBack.Draw(170, 990, 0, zoomSize);
		if (backBool == false) {
			backBool = true;			
			gs->MoveCursor.Play(false);
		}
	}
	else {
		backBool = false;
		gi->optionBack.Draw(170, 990);
	}


}
