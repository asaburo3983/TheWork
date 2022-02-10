#pragma once
#include "LowApplication.h"
#include "Camera.h"

namespace frauEngine {
	class Effect {
	private:

		EffekseerRenderer::RendererRef renderer;
		Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> sfMemoryPoolEfk;
		Effekseer::RefPtr<EffekseerRenderer::CommandList> commandListEfk;
		Effekseer::ManagerRef manager;
		Effekseer::EffectRef effect;
		int time=0;
		Effekseer::Handle handle = 0;
	public:
		~Effect();
		void Load(const char16_t* ufilepath);
		void Play(Vector3 _pos, Vector3 _rota, Vector3 _scale, int _endTime);
		void SetTime(int _time);
	};
}