#pragma once
#include "LowApplication.h"

#include "Object.h"

#include "Shader.h"

#include "Scene.h"

#include "ImGuiDX12.h"

#include "PostEffect.h"


namespace frauEngine {
	class Application {
	private:

		Scene* scene;
		Scene* sceneOld=nullptr;
		string sceneStr = "";
		string sceneOldStr = "";

		std::unordered_map<std::string, Scene*> sceneList;
		
		int loadTime = 0;//ƒ[ƒh’†‚©‚Ç‚¤‚©
		bool loaded = false;
		int loadTimeMaxFrame = 15;

		PostEffect postEffect;
		PostEffect blurEffect;
		PostEffect depthOfFieldEffect;

		bool depthOfField = false;

	public:
		static Application* GetInstance()
		{
			static Application happ;
			return &happ;
		}
		void Init(const char* _windowName, int _windowSizeX, int _windowSizeY, bool _fullscreen);
		void UnInit();
		void SetSceneList(std::string _str, Scene* _scene);
		void Load(std::string _nextScene);
		void Loop();
		void LoadLoop();
		void LoadScene();
		string GetSceneOldStr() { return sceneOldStr; };

		void SetLoadTimeMaxFrame(int _maxFrame) { loadTimeMaxFrame = _maxFrame; }

		void SetDepthOfField(bool _on, int forcusX = 0, int forcusY = 0) {
			auto lowApp = frauEngine::LowApplication::GetInstance();
			depthOfField = _on;
			depthOfFieldEffect.constantBufferHeap.buffer->forcusU = 1.0f / lowApp->GetWindowSizeX() * forcusX;
			depthOfFieldEffect.constantBufferHeap.buffer->forcusV = 1.0f / lowApp->GetWindowSizeY() * forcusY;
		}
		void SetBlurPower(float _power) {
			if (_power > 1) {
				_power = 1;
			}
			if (_power < 0) {
				_power = 0;
			}
			depthOfFieldEffect.constantBufferHeap.buffer->blurPower = _power;
		}
		//”íŽÊŠE[“x‚É’Ç‰Á‚Å“ü‚ê‚é‚±‚Æ‚É‚È‚é
		void SetAddEffect(int _effectNum) {
			depthOfFieldEffect.constantBufferHeap.buffer->effectNum = _effectNum;
		}
		void SetColorPlus(Color _color = { 0,0,0,0 }) {  depthOfFieldEffect.SetColorPlus(_color); }
		void SetColorMulti(Color _color = { 1,1,1,1 }) { depthOfFieldEffect.SetColorMulti(_color); }

		ID3D12DescriptorHeap* GetPostEffectRenderTarget() {
			return postEffect.GetRenderTargetHeap().Get();
		}
	private:
		void InitExternalLibrary();
		void UnInitExternalLibrary();

	};
}