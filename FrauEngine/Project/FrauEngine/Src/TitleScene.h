#pragma once
#include "Application.h"
#include "Object.h"

#include "Loading.h"

#include "MainGameCommonProcess.h"
#include "Save.h"

using namespace frauEngine;



class TitleScene : public Scene {
private:
	Loading loading;
	
	ImageObject titleFront;
	ImageObject titleBack;
	ImageObject titleCursor;
	ImageObject titleCloud;

	enum class TitleStr {
		START = 0,
		GALLARY = 1,
		OPTION = 2,
		EXIT = 3,
		MAX = 4
	};
	
	ImageObject titleStr[(int)TitleStr::MAX];



	const int strImageMoveStartX = 200;
	const int strImageMoveEndX = strImageMoveStartX + 100;
	const int strImageStartY = 760;
	const int strImageDistY = 80;
	float strImageLerp[(int)TitleStr::MAX] = { 1,0,0,0 };
	const float strImageLerpSpeed = 0.1f;

	const int cursorX = 400;
	int cursorNum = 0;

	bool fadeOn = false;
	bool sceneMove = false;
public:

	void LoadFrontLoad();
	void LoadInUpdate();
	void LoadInDraw();

	void StaticLoad();
	void Load();
	void Update();
	void Draw();
	void DrawNonePostEffect();
	void UnLoad();

};
