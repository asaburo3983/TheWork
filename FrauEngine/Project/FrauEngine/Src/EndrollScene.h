#pragma once
#include "Application.h"

#include "Objects.h"

#include "Loading.h"

#include "MainGameCommonProcess.h"

using namespace frauEngine;

class EndrollScene : public Scene {
private:
	Loading loading;

private:

	ImageObject black;
	
	Vector2 startPos = { 100,1500 };
	Vector2 stringDist = { 600,200 };

	int scrollCount = 0;
	int scrollCountSpeed = 2;
	int scrollCountMax = 1080 * 4;

	CSV csv;
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
