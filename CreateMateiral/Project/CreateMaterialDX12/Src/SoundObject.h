#pragma once
#include "Object.h"
#include "ConstantBuffer.h"
#include "ShaderResourceBuffer.h"
#include "Shader.h"
#include "DXTK.h"

namespace obj {

	class SoundObject : public obj::Object {
	protected:
		bool enable = true;
		dxtk::Sound_MP3* resource;
		bool loop;
		float volume;
	public:
		void SetResource(dxtk::Sound_MP3* _sound);

		void SetLoop(bool _loop);
		void SetVolume(float _volume);
		void SetAll(bool _loop, float _volume);
		void Stop();
		void Play();

		bool GetEnable() { return enable; };
		bool GetLoop() { return loop; };
		float GetVolume() { return volume; };

		void DrawImGUI();
		void Updata();
		void Draw();
	};
}