#include "CameraObject.h"

using namespace obj;
using namespace lowApp;

void CameraObject::SetPos(Vector3 _pos) {
	if (imgui == true) {
		return;
	}
	pos = _pos;
}
void CameraObject::SetTarget(Vector3 _target) {
	if (imgui == true) {
		return;
	}
	target = _target;
}
void CameraObject::SetAngle(Vector3 _angle) {
	if (imgui == true) {
		return;
	}
	angle = _angle;
}
void CameraObject::SetUp(Vector3 _up) {
	if (imgui == true) {
		return;
	}
	up = _up;
}
void CameraObject::SetFov(float _fov){
	if (imgui == true) {
		return;
	}
	fov = _fov;
}
void CameraObject::SetAll(Vector3 _pos, Vector3 _angle) {
	pos = _pos;
	angle = _angle;

}

void CameraObject::Updata() {

	camera3D::Camera::GetInstance()->SetCamera(pos, target, up, fov);
	
}
void CameraObject::DrawImGUI() {
	if (imgui == false) {
		return;
	}
	auto lowApp = LowApplication::GetInstance();


	ImGui::Begin(name);//ウィンドウ名になる


	float v3[3];
	ImGui::Checkbox("Enable", &enable);

	v3[0] = pos.X; v3[1] = pos.Y; v3[2] = pos.Z;
	ImGui::InputFloat3("Position", v3);
	pos.X = v3[0]; pos.Y = v3[1]; pos.Z = v3[2];

	v3[0] = angle.X; v3[1] = angle.Y; v3[2] = angle.Z;
	ImGui::InputFloat3("Angle", v3);
	angle.X = v3[0]; angle.Y = v3[1]; angle.Z = v3[2];

	v3[0] = target.X; v3[1] = target.Y; v3[2] =target.Z;
	ImGui::InputFloat3("Target", v3);
	target.X = v3[0]; target.Y = v3[1]; target.Z = v3[2];

	v3[0] = up.X; v3[1] = up.Y; v3[2] =up.Z;
	ImGui::InputFloat3("Up", v3);
	up.X = v3[0]; up.Y = v3[1]; up.Z = v3[2];

	ImGui::InputFloat("Fov", &fov);


	ImGui::End();
}
void CameraObject::Draw() {
	DrawImGUI();
	if (enable==false) {
		return;
	}

	camera3D::Camera::GetInstance()->SetCamera(pos, target, up, fov);


}
