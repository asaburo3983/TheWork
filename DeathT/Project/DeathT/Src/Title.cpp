#include "GameManager.h"

namespace TitleVariable {
	int titleCount = 0;
	float feedCount = 0; 
	int feedSpeed = 2;
	int cursorNum = 1;
	int cursorNumOld = 0;
	//文字エフェクト系
	int zoomCount = 0;
	bool zoomBool = true;
	const int zoomCountMax = 10;
	float zoomSize = 1;
	const int zoomSpeed = 6;

	MODEL_DATA player_MD;
	MODEL_DATA block_MD[2];
}
using namespace TitleVariable;

void GameManager::TitleLoad() {
	titleCount = 0;
	feedCount = 0;
	cursorNum = 0;
	cursorNumOld = 0;
	zoomCount = 0;
	zoomSize = 1;

	gs->Enter.Volume(seVolume);

}
void GameManager::TitleUpdata() {
	

	if (titleCount < 360) {
		titleCount++;
	}
	else {
		titleCount = 0;
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
	if (feedCount == 0) {
		//カーソル移動
		if (gi->startStr.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 1;

		}
		else if (gi->optionStr.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 2;

		}
		else if (gi->endStr.Hit(MOUSE->x, MOUSE->y)) {
			cursorNum = 3;

		}
		if (cursorNumOld != cursorNum) {
			zoomCount = 0;	
			gs->MoveCursor.Play(false);
		}
		cursorNumOld = cursorNum;
	}
	//シーン移動処理
	if (feedCount == 0 && MOUSE->left==1&& cursorNum!=0) {
		feedCount = 1;
		
		gs->Enter.Play();
	}
	if (feedCount) {
		feedCount+=feedSpeed;
		if (feedCount >= 100) {
			feedCount = 0;
			gs->BGM[scene].Stop();

			switch (cursorNum) {
			case 1:
				scene = SELECT;
				break;
			case 2:
				scene = OPTION;
				break;
			case 3:
				gameEnd = true;
				break;
			}
			
			sceneOld = TITLE;
		}
	}


	player_MD.pos = Vector3(-1.5f, -0.2f, 0.0f);
	player_MD.degree = Vector3(90.0f, 225.0f + titleCount, 0.0f);
	player_MD.scale = Vector3(0.5f, 0.5f, 0.5f);

	block_MD[0].pos = Vector3(-0.1, -4.0f + (float)titleCount / 50.0, 0.0f);
	block_MD[0].scale = Vector3(0.5f, 0.5f, 0.5f);
	block_MD[0].degree = Vector3(90.0f, 45.0f + titleCount, 0.0f);

	block_MD[1].pos = Vector3(-2.5, -4.5f + (float)titleCount / 50.0, 1.0f);
	block_MD[1].scale = Vector3(0.5f, 0.5f, 0.5f);
	block_MD[1].degree = Vector3(90.0f, 180.0f + titleCount, 0.0f);

}

void GameManager::TitleDraw() {
	DirectGraphics::GetInstance()->SetLight(Vector3(0, -4, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
	
	gm->Player->RenderShadowMap(&player_MD);

	gm->Block->RenderShadowMap(&block_MD[0]);
	gm->Block->RenderShadowMap(&block_MD[1]);

	DirectGraphics::GetInstance()->SetCamera(Vector3(0, -2, -4), Vector3(0, -1, 0), Vector3(0, 0, 0.1));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

	gi->title.Draw(960, 540);


	//選択肢文字表示表示
	switch (cursorNum) {
	case 1:
		gi->startStr.Draw(550, 740, 0, zoomSize);
		break;
	case 2:
		gi->optionStr.Draw(550, 880, 0, zoomSize);
		break;
	case 3:
		gi->endStr.Draw(550, 1000, 0, zoomSize);
		break;
	}
	if (cursorNum != 1)
		gi->startStr.Draw(550, 740, 0, 1);
	if (cursorNum != 2)
		gi->optionStr.Draw(550, 880, 0, 1);
	if (cursorNum != 3)
		gi->endStr.Draw(550, 1000, 0, 1);


	gm->Player->RenderShadowModel(&player_MD);

	gm->Block->RenderShadowModel(&block_MD[0]);
	gm->Block->RenderShadowModel(&block_MD[1]);

	gi->feedBlack.Draw(960, 540, 0, 1, feedCount / 100.0f);

	//gi->feedBlack.Draw(960, 540, 0, 1, 0.5f);
}

