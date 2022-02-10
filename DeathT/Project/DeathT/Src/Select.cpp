#include "GameManager.h"

namespace SelectVariable {

	int backCount = 0;
	const int backCountSpeed = 5;

	int cursorNum = 0;
	int cursorNumOld = 0;
	//エフェクト系
	int zoomCount = 0;
	bool zoomBool = true;
	const int zoomCountMax = 10;
	float zoomSize = 1;
	const int zoomSpeed = 6;

	bool startFade = true;
	bool endFade = false;
	float feedCount = 100;
	int feedSpeed = 2;
}
using namespace SelectVariable;
void GameManager::SelectLoad() {


	cursorNum = 0;
	cursorNumOld = 0;

	backCount = 0;

	zoomCount = 0;
	zoomSize = 1;
	feedCount = 100;
	startFade = true;
	endFade = false;
}
void GameManager::SelectUpdata() {
	backCount += backCountSpeed;

	//フェード処理
	if (startFade) {
		feedCount-=feedSpeed;
		if (feedCount <= 0) {
			startFade = false;
		}
	}
	if (endFade) {
		feedCount+=feedSpeed;
		if (feedCount >= 100) {
			gs->BGM[scene].Stop();

			switch (cursorNum) {
			case 1:
				scene = BATTLE;
				break;
			case 2:
				//scene = TOWER;
				
					scene = CLEDIT;
				break;
			case 3:
				scene = TITLE;
				break;
			case 4:
				scene = OPTION;
				break;
			}

			sceneOld = SELECT;
		}
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

	if (feedCount <= 0) {
		//カーソル移動
		if (gi->selectBattle.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 1;
		}
		else if (gi->selectTower.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 2;
		}
		else if (gi->selectTitle.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 3;
		}
		else if (gi->selectOption.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 4;		
		}
		if (cursorNumOld != cursorNum) {
			gs->MoveCursor.Play(false);
			zoomCount = 0;
		}
		cursorNumOld = cursorNum;
	}

	//シーン移動処理
	if (feedCount <= 0 && MOUSE->left == 1 && cursorNum != 0) {
		if (cursorNum == 2 && credit == false) {
		}
		else {
			endFade = true;
			gs->Enter.Play();
		}
	}

}
void GameManager::SelectDraw() {
	//ゲームの背景
	int gameBackX = 960 - backCount % 1920;
	int gameBackY = 540 + backCount % 1080;

	gi->select.Draw(gameBackX, gameBackY - 1080);
	gi->select.Draw(gameBackX, gameBackY);
	gi->select.Draw(gameBackX + 1920, gameBackY);
	gi->select.Draw(gameBackX + 1920, gameBackY - 1080);


	//選択肢文字表示表示
	float selectUISize[4] = { 1,1,1,1 };

	const int angleMove1 = -10;
	const int angleMove2 = 20;
	if(cursorNum!=0)
		selectUISize[cursorNum - 1] = zoomSize;
	
	gi->selectBattle.Draw(960, 1080/4, selectUISize[0]* angleMove1 - angleMove1);
	if(credit)
		gi->selectTower.DrawEX(1250, 1080 / 4 * 3, selectUISize[1] * angleMove1 - angleMove1, 0.66);
	else
		gi->selectTower.DrawEX(1250, 1080 / 4 * 3, selectUISize[1] * angleMove1 - angleMove1, 0.66,1,0.5);
	gi->selectTitle.Draw(315, 700, selectUISize[2] * angleMove2 - angleMove2);
	gi->selectOption.Draw(315, 930, selectUISize[3] * angleMove2 - angleMove2);

	gi->feedBlack.Draw(960, 540, 0, 1, feedCount / 100.0f);
}

