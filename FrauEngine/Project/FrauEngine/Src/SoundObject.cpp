#include "SoundObject.h"

using namespace frauEngine;

void SoundObject::SetResource(Sound_MP3* _sound) {
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
		resource->Volume(volume * volumeMulti);
		return;
	}
	volume = _volume;
	resource->Volume(volume * volumeMulti);
}
void SoundObject::AddVolume(float _volume) {
	if (imgui == true) {
		resource->Volume(volume * volumeMulti);
		return;
	}
	if (_volume > 0 && volume < 1.0f) {
		volume += _volume;
	}
	else if (_volume < 0 && volume > 0) {
		volume += _volume;
	}
	resource->Volume(volume * volumeMulti);
}
void SoundObject::SetVolumeMulti(float _volume) {
	if (imgui == true) {
		resource->Volume(volume * volumeMulti);
		return;
	}
	volumeMulti = _volume;
	resource->Volume(volume * volumeMulti);
}
void SoundObject::AddVolumeMulti(float _volume) {
	if (imgui == true) {
		resource->Volume(volume * volumeMulti);
		return;
	}
	if (_volume > 0 && volumeMulti < 1.0f) {
		volumeMulti += _volume;
	}
	else if(_volume < 0 && volumeMulti > 0){
		volumeMulti += _volume;
	}
	resource->Volume(volume * volumeMulti);
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
void SoundObject::Update() {
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
