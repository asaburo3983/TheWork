#pragma once
#include "GameMathLib.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include <memory>
#include "Window.h"
#include "DirectGraphics.h"
#include "FbxMeshFile.h"
#define FBXSDK_SHARED
#include "DXTK11Lib.h"
#include "input.h"

using namespace GameMath;

void AllInit(const char* winName, int sizeX, int sizeY,bool fullScreen, HINSTANCE hInstance);

void SetAntialias(bool on);

class MODEL_DATA {
public:
	Vector3 pos;
	Vector3 degree;
	Vector3 scale;
	Vector3 axis;
	float axisAngle;
	Vector3 axisR;
	DirectX::XMFLOAT4  multiplyColor;

public:
	MODEL_DATA();

	void DebugMode(float speed = 0.01,bool quaternion=false, int strX = 0, int strY = 0);

};

class FBX_MODEL : public FbxMeshFile {
public:
	FBX_MODEL* modelData;
	int animeNum = NULL;
public:
	FBX_MODEL();
	void SetScaleALL(float _scale);
	void RenderShadowMap(MODEL_DATA* modelData);
	void RenderShadowModel(MODEL_DATA* modelData);
	void RenderModel(MODEL_DATA* modelData);
	void SetModelData(MODEL_DATA* modelData);
private:

};

class CAMERA {

public:
	static CAMERA* GetInstance() {
		static CAMERA cam;
		return &cam;
	}
private:
	
	const float length = 10.0f;
public:
	Vector3 pos;
	Vector3 angle;
	Vector3 focs;
public:
	CAMERA();
	void Updata(bool local);
	void Debug(float speed = 1,int strX=0, int strY=0);
};
