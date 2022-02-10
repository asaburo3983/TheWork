#include "LightsObject.h"

using namespace frauEngine;

void LightsObject::SetAmbientLight(float _power) {
	ambientPower = _power;
}

void LightsObject::SetDirectionalLight(DirectionalLightParam _param) {
	directionalLightParam = _param;
}

void LightsObject::SetPointLight(int _lightNum, PointLightParam _param) {
	pointLightParam[_lightNum] = _param;
}
void LightsObject::SetSpotLight(int _lightNum, SpotLightParam _param) {
	spotLightParam[_lightNum] = _param;
}

void LightsObject::DrawImGUI() {

	if (imgui == false) {
		return;
	}

	float v3[3];

	ImGui::Begin("DirectionalLight");//ウィンドウ名になる

	directionalLightParam.pos.GetFloat3(v3);
	ImGui::InputFloat3("Position", v3);
	directionalLightParam.pos.SetFloat3(v3);

	directionalLightParam.target.GetFloat3(v3);
	ImGui::InputFloat3("Target", v3);
	directionalLightParam.target.SetFloat3(v3);

	directionalLightParam.up.GetFloat3(v3);
	ImGui::InputFloat3("Up", v3);
	directionalLightParam.up.SetFloat3(v3);

	ImGui::InputFloat("Fov", &directionalLightParam.fov);

	directionalLightParam.vector.GetFloat3(v3);
	ImGui::InputFloat3("Vector", v3);
	directionalLightParam.vector.SetFloat3(v3);

	directionalLightParam.color.GetFloat3(v3);
	ImGui::ColorEdit3("Color", v3);
	directionalLightParam.color.SetFloat3(v3);

	ImGui::End();

	ImGui::Begin("PointLights");
	for (int i = 0; i < POINT_LIGHT;i++) {
		char tmpName[256];
		sprintf_s(tmpName, "Light%d", i);
		ImGui::Text(tmpName);

		sprintf_s(tmpName, "Pos%d", i);
		pointLightParam[i].pos.GetFloat3(v3);
		ImGui::InputFloat3(tmpName, v3);
		pointLightParam[i].pos.SetFloat3(v3);

		sprintf_s(tmpName, "Range%d", i);
		ImGui::InputFloat(tmpName, &pointLightParam[i].range);

		sprintf_s(tmpName, "Color%d", i);
		pointLightParam[i].color.GetFloat3(v3);
		ImGui::ColorEdit3(tmpName, v3);
		pointLightParam[i].color.SetFloat3(v3);
	}
	ImGui::End();

	ImGui::Begin("SpotLights");
	for (int i = 0; i < SPOT_LIGHT;i++) {
		char tmpName[256];
		sprintf_s(tmpName, "Light%d", i);
		ImGui::Text(tmpName);

		sprintf_s(tmpName, "Pos%d", i);
		spotLightParam[i].pos.GetFloat3(v3);
		ImGui::InputFloat3(tmpName, v3);
		spotLightParam[i].pos.SetFloat3(v3);

		sprintf_s(tmpName, "Range%d", i);
		ImGui::InputFloat(tmpName, &spotLightParam[i].range);

		sprintf_s(tmpName, "Target%d", i);
		spotLightParam[i].target.GetFloat3(v3);
		ImGui::InputFloat3(tmpName, v3);
		spotLightParam[i].target.SetFloat3(v3);

		sprintf_s(tmpName, "AnglerRange%d", i);
		ImGui::InputFloat(tmpName, &spotLightParam[i].anglerRange);

		sprintf_s(tmpName, "Color%d", i);
		spotLightParam[i].color.GetFloat3(v3);
		ImGui::ColorEdit3(tmpName, v3);
		spotLightParam[i].color.SetFloat3(v3);

	}
	ImGui::End();

}

void LightsObject::Update() {

	
	auto light = Lights::GetInstance();

	DrawImGUI();

	light->SetAmbientLight(ambientPower);

	light->SetDirectionalLight(
		directionalLightParam.pos,
		directionalLightParam.target,
		directionalLightParam.up,
		directionalLightParam.fov,
		directionalLightParam.vector,
		directionalLightParam.color);

	for (int i = 0; i < POINT_LIGHT; i++) {
		light->SetPointLight(i,
			pointLightParam[i].pos,
			pointLightParam[i].range,
			pointLightParam[i].color);
	}
	for (int i = 0; i < SPOT_LIGHT; i++) {
		light->SetSpotLight(i,
			spotLightParam[i].pos,
			spotLightParam[i].range,
			spotLightParam[i].target,
			spotLightParam[i].anglerRange,
			spotLightParam[i].color);
	}
}