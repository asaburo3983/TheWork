#include "SimpleBoxCollider.h"
#include <utility>
SimpleBoxCollider2D::SimpleBoxCollider2D() {

}
SimpleBoxCollider2D::SimpleBoxCollider2D(Vector2 _pos, Vector2 _scale) {
	Setup( _pos, _scale);
}
void SimpleBoxCollider2D::Setup(Vector2 _pos, Vector2 _scale, int _eventNum) {
	pos = _pos;
	scale = _scale;

	point[0].X = _pos.X - 0.5 * scale.X;
	point[1].X = _pos.X + 0.5 * scale.X;
	
	point[0].Y = _pos.Y - 0.5 * scale.Y;
	point[1].Y = _pos.Y + 0.5 * scale.Y;
	
	eventNum = _eventNum;
}
void SimpleBoxCollider2D::Setup(float _left, float _right, float _up, float _down, int _eventNum) {
	point[0].X = _left;
	point[1].X = _right;
	if (point[0].X > point[1].X) {
		std::swap(point[0].X, point[1].X);
	}
	point[0].Y = _up;
	point[1].Y = _down;
	if (point[0].Y > point[1].Y) {
		std::swap(point[0].Y, point[1].Y);
	}
	eventNum = _eventNum;
}
bool SimpleBoxCollider2D::HitX(float _posX) {
	if (point[0].X <= _posX && _posX <= point[1].X) {
		return true;
	}
	return false;
}
bool SimpleBoxCollider2D::HitY(float _posY) {
	if (point[0].Y <= _posY && _posY <= point[1].Y) {
		return true;
	}
	return false;
}

bool SimpleBoxCollider2D::Hit(Vector2 _pos) {
	if (!HitX(_pos.X)) {
		return false;
	}
	if (!HitY(_pos.Y)) {
		return false;
	}
	return true;
}

