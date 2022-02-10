#include "EffectObject.h"

//イメージ
using namespace obj;
//エフェクト
void EffectObject::SetResource(efk::Effect* _effect) {
	resource = _effect;
}
void EffectObject::SetPos(Vector3 _pos) {
	if (imgui == true) {
		return;
	}
	pos = _pos;
}
void EffectObject::SetScale(Vector3 _scale) {
	if (imgui == true) {
		return;
	}
	scale = _scale;
}
void EffectObject::SetAngle(Vector3 _angle) {
	if (imgui == true) {
		return;
	}
	angle = _angle;
}
void EffectObject::SetTime(int _time) {
	if (imgui == true) {
		return;
	}
	time = _time;
}
void EffectObject::SetEndTime(int _endTime) {
	if (imgui == true) {
		return;
	}
	endTime = _endTime;
}
void EffectObject::SetAll(Vector3 _pos, Vector3 _angle, Vector3 _scale, int _endTime) {
	if (imgui == true) {
		return;
	}
	SetPos(_pos);
	SetAngle(_angle);
	SetScale(_scale);
	SetEndTime(_endTime);
}
void EffectObject::Play() {
	if (enable == false) {
		return;
	}
	resource->Play(pos, angle, scale, endTime);
}

void EffectObject::DrawImGUI() {
	if (imgui == false) {
		return;
	}

	ImGui::Begin(name);//ウィンドウ名になる


	float v3[3];
	ImGui::Checkbox("Enable", &enable);

	v3[0] = pos.X; v3[1] = pos.Y; v3[2] = pos.Z;
	ImGui::InputFloat3("Position", v3);
	pos.X = v3[0]; pos.Y = v3[1]; pos.Z = v3[2];

	v3[0] = angle.X; v3[1] = angle.Y; v3[2] = angle.Z;
	ImGui::InputFloat3("Angle", v3);
	angle.X = v3[0]; angle.Y = v3[1]; angle.Z = v3[2];

	v3[0] = scale.X; v3[1] = scale.Y; v3[2] = scale.Z;
	ImGui::InputFloat3("Scale", v3);
	scale.X = v3[0]; scale.Y = v3[1]; scale.Z = v3[2];

	ImGui::InputInt("EndTime", &endTime);

	ImGui::End();


}
void EffectObject::Updata() {
	if (enable == false) {
		return;
	}
}
void EffectObject::Draw() {
	DrawImGUI();
	if (enable == false) {
		return;
	}
	Play();
	//resource.effect->Play(pos, angle, endTime);
}

