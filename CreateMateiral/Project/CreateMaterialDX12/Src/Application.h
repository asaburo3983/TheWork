#pragma once
#include "LowApplication.h"

#include "Object.h"

#include "Shader.h"

#include "Scene.h"

#include "ImGuiDX12.h"

#include "PostEffect.h"


namespace app {
	class Application {
	private:

		Scene* scene;
		Scene* sceneOld;

		bool load = true;//ƒ[ƒh’†‚©‚Ç‚¤‚©

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
		void Load(Scene* _scene);
		void Loop();
		void LoadLoop();
		void LoadScene();

		void SetDepthOfField(bool _on, int forcusX = 0, int forcusY = 0) {
			auto lowApp = lowApp::LowApplication::GetInstance();
			depthOfField = _on;
			depthOfFieldEffect.constantBufferHeap.buffer->forcusU = 1.0f / lowApp->GetWindowSizeX() * forcusX;
			depthOfFieldEffect.constantBufferHeap.buffer->forcusV = 1.0f / lowApp->GetWindowSizeY() * forcusY;
		}

		ID3D12DescriptorHeap* GetPostEffectRenderTarget() {
			return postEffect.GetRenderTargetHeap().Get();
		}
	private:
		void InitExternalLibrary();
		void UnInitExternalLibrary();

	};
}