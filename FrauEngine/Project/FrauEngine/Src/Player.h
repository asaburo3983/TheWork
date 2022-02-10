#pragma once
#include "Resource.h"

#include "ModelExtendObject.h"
#include "SimpleBoxCollider.h"
#include "SimpleCircleCollider.h"

#include "MaterialLinkerData.h"

#include "Stage.h"

using namespace frauEngine;


class Player {

private:
	KeyInput* KEY;

	ModelExtendObject model;
	
	enum class IdeaNum {
		EXIT,
		PLANTER,
		OPEN_SHOP,
		BUY,
		JOIN_SHOP,
		MAX
	};
	ModelObject ideaFrame;
	ModelObject ideaStr[(int)IdeaNum::MAX];


	int eventNum = 0;
	bool isMove= true;
	bool isAnimation = true;
	bool isCollision = true;

	Vector3 posOld;
	float moveSpeed;
	float rotaSpeed;

	int sitAnimCount = 0;
	const int sitAnimCountMax = 60;

	int walkSoundCount = 0;
	const int walkSoundCountMax = 30;

public:
	static Player* GetInstance() {
		static Player player;
		return &player;
	}
	Player();
	void Initialize(float _moveSpeed, float _rotaSpeed);

	void MoveStage();
	
	void Update();
	void DrawShadow();
	void Draw();
	void DrawBillBoard();

	void IsMove(bool _enable) { 
		isMove = _enable;
		posOld = model.GetPos();
	}
	void IsAnimation(bool _enable) { isAnimation = _enable; }
	void IsCollision(bool _enable) { isCollision = _enable; }

	void SetPos(Vector3 _pos) { model.SetPos(_pos); }
	void SetAngle(Vector3 _angle) { model.SetAngle(_angle); }
	Vector3 GetPos() { return model.GetPos(); }
	Vector3 GetPosOld(){ return posOld; }
	int GetEventNum(){ return eventNum; }

	ModelExtendObject* GetModel() { return &model; };

	void Reset() {
		eventNum = 0;
		isMove = true;
		isAnimation = true;
		isCollision = true;

		sitAnimCount = 0;

		walkSoundCount = 0;
	}
private:
	void Move();
	void SetVector(float _target,float _targetX,float _targetY);
	void AnimationControl();
	void Collision();

	void MoveBackX() {
		Vector3 posOldX = model.GetPos();
		posOldX.X = posOld.X;
		model.SetPos(posOldX);
	}
	void MoveBackZ() {
		Vector3 posOldZ = model.GetPos();
		posOldZ.Z = posOld.Z;
		model.SetPos(posOldZ);
	}

	void SetStartPos();

};
