#pragma once
#include "Application.h"

#include "Objects.h"

#include "Loading.h"

#include "MagicShop.h"
#include "MainGameCommonProcess.h"


using namespace frauEngine;

class MagicShopScene : public Scene {
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
