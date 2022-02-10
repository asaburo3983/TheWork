#pragma once
#include "Application.h"
#include "Object.h"

#include "Loading.h"

#include "MainGameCommonProcess.h"
using namespace frauEngine;



class OptionScene : public Scene {
private:
	Loading loading;

	ImageObject backSceneButton;
	ImageObject backTitleButton;

	ImageObject base;
	ImageObject bar[3];
	ImageObject miniButton[3];
	
	float volumes[3] = { 1.0,0.5,0.5 };

	int barLeftX = 0;
	int barRightX = 0;

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
