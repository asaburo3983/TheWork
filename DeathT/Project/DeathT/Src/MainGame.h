#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include "Window.h"
#include "DirectGraphics.h"
#include "FbxMeshFile.h"
#define FBXSDK_SHARED
#pragma comment(lib, "winmm.lib")

#include "DXTK11Lib.h"
#include "input.h"
using namespace DXTK11LIB;
using namespace GameMath;

class GameModel {
public:
	static GameModel* GetInstance() {
		static GameModel gm;
		return &gm;
	}
	FbxMeshFile* Player;
	FbxMeshFile* Stage;
	FbxMeshFile* Block;
};
class GameStatic {

public:

	MouseInput* MOUSE;
	KeyInput* KEY;

public:

	static GameStatic* GetInstance() {
		static GameStatic gameSt;
		return &gameSt;
	}
	void Updata() {
		KEY = KeyInput::GetInstance();
		MOUSE = MouseInput::GetInstance();
	}
};


class Camera {
public:
	Pos3 pos;
};

class Light {
public:
	Pos3 pos;
};
void MainGameLoad();
void MainGameUpdata();
void MainGameDraw();
