#pragma once
#include "Object.h"
#include "ConstantBuffer.h"
#include "ShaderResourceBuffer.h"
#include "Shader.h"
#include "DXTK.h"

namespace frauEngine {

	class SoundObject : public Object {
	protected:
		bool enable = true;
		Sound_MP3* resource;
		bool loop=false;
		float volume=1.0f;
		float volumeMulti = 1.0f;
	public:
		void SetResource(Sound_MP3* _sound);

		void SetLoop(bool _loop);
		void SetVolume(float _volume);
		void AddVolume(float _volume);
		void SetVolumeMulti(float _volume);
		void AddVolumeMulti(float _volume);
		void SetAll(bool _loop, float _volume);
		void Stop();
		void Play();

		bool GetEnable() { return enable; };
		bool GetLoop() { return loop; };
		float GetVolume() { return volume; };
		float GetVolumeMulti() { return volumeMulti; };
		void DrawImGUI();
		void Update();
		void Draw();
	};
}