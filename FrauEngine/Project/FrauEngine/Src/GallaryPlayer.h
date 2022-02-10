#pragma once
#include "ModelExtendObject.h"
#include "SimpleBoxCollider.h"
#include "SimpleCircleCollider.h"

using namespace frauEngine;

class GallaryPlayer {

private:
	KeyInput* KEY;
	ModelExtendObject* resource;
	Vector3 posOld;

	int eventNum = 0;
public:
	static GallaryPlayer* GetInstance() {
		static GallaryPlayer player;
		return &player;
	}
	GallaryPlayer();
	void Setup(ModelExtendObject* _resource);
	void AnimationControl();
	void Move(float _moveSpeed, float _rotaSpeed);
	void Collision(vector<SimpleBoxCollider2D> _boxCollider, vector<SimpleCircleCollider2D> _circleCollider, vector<SimpleBoxCollider2D> _boxColliderEvent);

	void Draw(ModelType _modelType);


	Vector3 GetPos();
	Vector3 GetPosOld();
	int GetEventNum();

private:
	void MoveBackX();
	void MoveBackZ();

};
