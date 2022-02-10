#pragma once
#include "Library.h"

class SimpleCircleCollider2D {
private:
	Vector2 pos;
	float radius;
public:
	SimpleCircleCollider2D();
	SimpleCircleCollider2D(Vector2 _pos, float _radius);
	void Setup(Vector2 _pos, float _radius);
	bool Hit(Vector2 _pos, float _radius);
};