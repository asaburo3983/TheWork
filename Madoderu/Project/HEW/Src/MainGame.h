#pragma once
#include "DXTK11Lib.h"
#include "GameMAthLib.h"
#include "input.h"
#include "SaveLoadMap.h"
#include <vector>
#include<Shlwapi.h>

using namespace DXTK11LIB;
using namespace GameMath;

enum SCENE_NUM {
	TITLE,
	STAGESELECT,
	GAME,
	GAME_CLEAR,
	CREDIT
};
class GameSound {
public:

	Sound_MP3 BGM[5];
	Sound_MP3 SE[10];

	static GameSound* GetInstance() {
		static GameSound gameSo;
		return &gameSo;
	}

};
class GameImage {
public:
	
	Image title[7];

	Image stageSelect[10];

	Image staticIm[10];

	Image cursorIm[11];

	Image* mapTipIm;

	Image** player;

	Image effect[60];

	Image credit[10];
	static GameImage* GetInstance() {
		static GameImage gameIm;
		return &gameIm;
	}

	~GameImage() {
	}
};
class GameStatic {

public:
	int scene = TITLE;
	MouseInput* MOUSE;
	KeyInput* KEY;

	int stageSelect = -1;

	float mouseExtend = 1;

	bool moveTutolial = false;
	bool jumpTutolial = false;

	
public:
	void SetScene(SCENE_NUM num) { scene = num; };

	static GameStatic* GetInstance() {
		static GameStatic gameSt;
		return &gameSt;
	}
	void Updata() {
		KEY = KeyInput::GetInstance();
		MOUSE = MouseInput::GetInstance();
	}
};
///////////////////////
void MainGameLoad();

void MainGameUpdata();

void MainGameDraw();
//////////////////////
void TitleLoad();

void TitleUpdata();

void TitleDraw();
//////////////////////

void StageSelectLoad();

void StageSelectUpdata();

void StageSelectDraw();
//////////////////////

void GameLoad();

void GameUpdata();

void GameDraw();

void CreditLoad();
void CreditUpdata();
void CreditDraw();

void SetClear();

void SetClearStage(int num);