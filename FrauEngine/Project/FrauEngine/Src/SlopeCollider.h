#pragma once
#include "Library.h"

enum class SimpleVector {
	LEFT,
	RIGHT,
	UP,
	DOWN
};
class SlopeCollider {
private:
	Vector2 point[2];
	float bottomPos;
	float topPos;
	float lerpCount = 0.0f;
	SimpleVector topVector;
public:
	void Setup(float _left, float _right, float _up, float _down, float bottomPos, float topPos, SimpleVector _topVector);
	bool Hit(Vector2 _pos);
	bool HitX(float _posX);
	bool HitY(float _posY);

	float HitProcess(Vector2 _pos);
};