#pragma once
#include "Application.h"

#include "Objects.h"

#include "Loading.h"

#include "MainGameCommonProcess.h"

using namespace frauEngine;

class FlowerShopScene : public Scene {
private:
	Loading loading;

	bool IsPostEffect = true;
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
