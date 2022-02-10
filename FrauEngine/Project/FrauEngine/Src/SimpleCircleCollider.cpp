#include "SimpleCircleCollider.h"


SimpleCircleCollider2D::SimpleCircleCollider2D() {

}
SimpleCircleCollider2D::SimpleCircleCollider2D(Vector2 _pos, float _radius) {
	Setup(_pos, _radius);
}
void SimpleCircleCollider2D::Setup(Vector2 _pos, float _radius) {
	pos = _pos;
	radius = _radius;
}
bool SimpleCircleCollider2D::Hit(Vector2 _pos, float _radius) {
	
	GameMath::Pos2 p1;
	p1.p[0] = pos.X;
	p1.p[1] = pos.Y;
	
	GameMath::Pos2 p2;
	p2.p[0] = _pos.X;
	p2.p[1] = _pos.Y;

	return GameMath::HitCircle(p1, radius,p2, _radius);

}