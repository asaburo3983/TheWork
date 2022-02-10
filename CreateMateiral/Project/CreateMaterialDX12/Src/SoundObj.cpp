#include "SoundObject.h"
//イメージ
using namespace obj;

using namespace lowApp;

//サウンド
void SoundObject::SetResource(dxtk::Sound_MP3* _sound) {
	resource = _sound;
}
void SoundObject::SetLoop(bool _loop) {
	if (imgui == true) {
		return;
	}
	loop = _loop;
}
void SoundObject::SetVolume(float _volume) {
	if (imgui == true) {
		resource->Volume(volume);
		return;
	}
	volume = _volume;
	resource->Volume(volume);
}
void SoundObject::SetAll(bool _loop, float _volume) {
	if (imgui == true) {
		return;
	}
	SetLoop(_loop);
	SetVolume(_volume);
}
void SoundObject::Stop() {
	resource->Stop();
}
void SoundObject::Play() {
	resource->Play(loop);
}

void SoundObject::DrawImGUI() {
	if (imgui == false) {
		return;
	}
	LowApplication* lowApp = LowApplication::GetInstance();

	ImGui::Begin(name);//ウィンドウ名になる


	ImGui::Checkbox("Enable", &enable);

	ImGui::Checkbox("Loop", &loop);

	ImGui::SliderFloat("Volume", &volume, 0, 1.0f);

	ImGui::End();
}
void SoundObject::Updata() {
	if (enable == false) {
		Stop();
		return;
	}
}
void SoundObject::Draw() {
	DrawImGUI();
	if (enable == false) {
		Stop();
		return;
	}
	SetVolume(volume);
	Play();
}
