#include "SlopeCollider.h"


void SlopeCollider::Setup(float _left, float _right, float _up, float _down, float _bottomPos, float _topPos, SimpleVector _topVector) {
	point[0].X = _left;
	point[1].X = _right;
	if (point[0].X > point[1].X) {
		std::swap(point[0].X, point[1].X);
	}
	point[0].Y = _up;//[0]UP‚Ì‚Ù‚¤‚ª¬‚³‚¢(Žè‘O‚É‘¶Ý‚·‚é)TODO–ï‰î‚È‚Ì‚ÅŽ¡‚µ‚½‚¢
	point[1].Y = _down;
	if (point[0].Y > point[1].Y) {
		std::swap(point[0].Y, point[1].Y);
	}
	topVector = _topVector;

	topPos = _topPos;
	bottomPos = _bottomPos;
}
bool SlopeCollider::HitX(float _posX) {
	if (point[0].X <= _posX && _posX <= point[1].X) {
		return true;
	}
	return false;
}
bool SlopeCollider::HitY(float _posY) {
	if (point[0].Y <= _posY && _posY <= point[1].Y) {
		return true;
	}
	return false;
}

bool SlopeCollider::Hit(Vector2 _pos) {
	if (!HitX(_pos.X)) {
		return false;
	}
	if (!HitY(_pos.Y)) {
		return false;
	}
	return true;
}
float SlopeCollider::HitProcess(Vector2 _pos) {
	float ret = NULL;
	float slopeLengthX = abs(point[1].X - point[0].X);
	float slopeLengthY = abs(point[1].Y - point[0].Y);

	//_pos‚ª•‰‚Ì’l‚É‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚éˆ—
	if (point[0].X < 0) {
		_pos.X += -point[0].X;
	}
	if (point[0].Y < 0) {
		_pos.Y += -point[0].Y;
	} 
	switch (topVector) {
	case SimpleVector::LEFT:
		lerpCount = 1.0f-_pos.X / slopeLengthX;	
		break;
	case SimpleVector::RIGHT:
		lerpCount =  _pos.X / slopeLengthX;
		break;
	case SimpleVector::UP:
		lerpCount = (_pos.Y) / slopeLengthY;
		break;
	case SimpleVector::DOWN:
		lerpCount = 1.0f - _pos.Y / slopeLengthY;
		break;

	}

	ret = std::lerp(bottomPos, topPos, lerpCount);
	return ret;
}

