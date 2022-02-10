#pragma once
#include "Object.h"

class SimpleBoxCollider2D {
 private:
	 Vector2 pos;
	 Vector2 scale;

	 Vector2 point[2];

	 int eventNum = -1;
public:
	SimpleBoxCollider2D();
	SimpleBoxCollider2D(Vector2 _pos, Vector2 _scale);
	void Setup(Vector2 _pos, Vector2 _scale, int _eventNum = -1);
	void Setup(float _left, float __right, float _up, float _down,int _eventNum=-1);
	bool HitX(float _posX);
	bool HitY(float _posY);
	bool Hit(Vector2 _pos);
	int GetEventNum() { return eventNum; }
};