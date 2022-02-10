#include "ImageObject.h"

//イメージ
using namespace obj;
using namespace lowApp;

void ImageObject::SetResource(dxtk::Image2D* _image) {
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

	ImGui::Begin(name);                          //ウィンドウ名になる

	float v1;
	float v2[2];

	ImGui::Checkbox("Enable", &enable);//ブールを切り替えれる

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

	ImGui::Checkbox("turnLR", &turnLR);//ブールを切り替えれる
	ImGui::Checkbox("turnUD", &turnUD);//ブールを切り替えれる
	ImGui::Checkbox("center", &center);//ブールを切り替えれる



	ImGui::End();

}
void ImageObject::Updata() {
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

