#pragma once
#include "Application.h"

#include "Objects.h"

#include "Loading.h"

#include "Player.h"
#include "Stage.h"
#include "MaterialLinkerData.h"
#include "NovelSystem.h"
#include "CameraWork.h"
#include "EventManager.h"
#include "TutorialSystem.h"

#include "MainGameCommonProcess.h"
using namespace frauEngine;

class HandyShopScene : public Scene {
private:
	Loading loading;

private:


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
