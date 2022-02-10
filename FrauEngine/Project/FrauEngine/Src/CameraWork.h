#pragma once

#include "Application.h"

#include "CameraObject.h"
#include "Player.h"
using namespace frauEngine;

class CameraWork {
private:
	int moveNumOld = 0;
	int moveNum = 0;

	CameraObject camera;
	
	std::vector<Vector3> pos;
	std::vector <Vector3> target;


	float lerpCount = 0.0f;
	float lerpSpeed = 0.01f;

	bool playerTarget = false;
	bool photoMode = false;
public:
	static CameraWork* GetInstance() {
		static CameraWork cw;
		return &cw;
	}
	void Initialize();
	void SetMoveNum(int _moveNum);
	int GetMoveNum() { return moveNum; }
	void SetPlayerTarget(bool _enable) { playerTarget = _enable; }
	void Updata();

	void SetLerpSpeed(float _speed) { lerpSpeed = _speed; }

	CameraObject* GetCamera() { return &camera;}
	void SetPhotoMode(bool _enable);
	void Reset() {
		moveNumOld = 0;
		moveNum = 0;
		lerpCount = 0.0f;
		lerpSpeed = 0.01f;
		playerTarget = false;
		SetMoveNum(moveNum);
		SetPlayerTarget(playerTarget);
	}
private:
	void Move();
	void PlayerTarget();
	void PhotoMode();
};