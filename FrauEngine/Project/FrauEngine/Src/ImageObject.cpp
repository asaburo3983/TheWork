#include "ImageObject.h"

using namespace frauEngine;

void ImageObject::SetResource(frauEngine::Image2D* _image) {
	resource = _image;
}
void ImageObject::SetPos(Vector2 _pos) {
	if (imgui == true) {
		return;
	}
	pos = _pos;
}
void ImageObject::SetScale(Vector2 _scale) {
	if (imgui == true) {
		return;
	}
	scale = _scale;
}
void ImageObject::SetAngle(float _angle) {
	if (imgui == true) {
		return;
	}
	angle = _angle;
}
void ImageObject::SetAlpha(float _alpha) {
	if (imgui == true) {
		return;
	}
	alpha = _alpha;
}
void ImageObject::SetTurn(bool _turnLR, bool _turnUD) {
	if (imgui == true) {
		return;
	}
	turnLR = _turnLR;
	turnUD = _turnUD;

}
void ImageObject::SetCenter(bool _center) {
	if (imgui == true) {
		return;
	}
	center = _center;
}
void ImageObject::SetAll(Vector2 _pos, Vector2 _scale, float _angle, float _alpha) {
	if (imgui == true) {
		return;
	}
	SetPos(_pos);
	SetScale(_scale);

	SetAngle(_angle);

	SetAlpha(_alpha);

}

void ImageObject::DrawImGUI() {
	if (imgui == false) {
		return;
	}
	LowApplication* lowApp = LowApplication::GetInstance();

	ImGui::Begin(name);                          //�E�B���h�E���ɂȂ�

	float v1;
	float v2[2];

	ImGui::Checkbox("Enable", &enable);//�u�[����؂�ւ����

	v2[0] = pos.X; v2[1] = pos.Y;
	ImGui::InputFloat2("Position", v2);
	pos.X = v2[0]; pos.Y = v2[1];

	v2[0] = scale.X; v2[1] = scale.Y;
	ImGui::InputFloat2("Scale", v2);
	scale.X = v2[0]; scale.Y = v2[1];

	v1 = angle;
	ImGui::InputFloat("Angle", &v1);
	angle = v1;
	v1 = alpha;
	ImGui::SliderFloat("Alpha", &v1, 0, 1.0);
	alpha = v1;

	ImGui::Checkbox("turnLR", &turnLR);//�u�[����؂�ւ����
	ImGui::Checkbox("turnUD", &turnUD);//�u�[����؂�ւ����
	ImGui::Checkbox("center", &center);//�u�[����؂�ւ����



	ImGui::End();

}
void ImageObject::Update() {
	if (enable == false) {
		return;
	}
}
void ImageObject::Draw() {
	DrawImGUI();
	if (enable == false) {
		return;
	}
	resource->Draw(pos.X, pos.Y, scale.X, scale.Y, angle, alpha, turnLR, turnUD, center);
}

bool ImageObject::Hit(int _x,int _y) {

	using namespace GameMath;
	
	float width = resource->GetWidth() * scale.X;
	float height = resource->GetHeight() * scale.Y;

	return HitRect(Pos2{ (float)pos.X, (float)pos.Y }, Pos2{ width,height }, Pos2{ (float)_x,(float)_y });
}