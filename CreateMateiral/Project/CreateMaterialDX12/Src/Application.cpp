#include "Application.h"

//vector str;
using namespace lowApp;




void app::Application::Init(const char* _windowName, int _windowSizeX, int _windowSizeY, bool _fullscreen) {

	lowApp::LowApplication::GetInstance()->SetDebugMode(true);

	lowApp::LowApplication::GetInstance()->Init(_windowName, _windowSizeX, _windowSizeY, _fullscreen);

	InitExternalLibrary();

	auto resource = Resource::GetInstance();
	resource->LoadShader(shader::ShaderType::VS, "Data/Shader/VertexShader_PostEffect.hlsl");
	resource->LoadShader(shader::ShaderType::PS, "Data/Shader/PixelShader_PostEffect.hlsl");

	resource->LoadShader(shader::ShaderType::PS, "Data/Shader/PixelShader_Blur.hlsl");
	resource->LoadShader(shader::ShaderType::PS, "Data/Shader/PixelShader_DepthOfField.hlsl");

	postEffect.Create(
		resource->Shader("Data/Shader/VertexShader_PostEffect.hlsl"),
		resource->Shader("Data/Shader/PixelShader_PostEffect.hlsl"));
	blurEffect.Create(
		resource->Shader("Data/Shader/VertexShader_PostEffect.hlsl"),
		resource->Shader("Data/Shader/PixelShader_Blur.hlsl"));

	depthOfFieldEffect.Create(
		resource->Shader("Data/Shader/VertexShader_PostEffect.hlsl"),
		resource->Shader("Data/Shader/PixelShader_DepthOfField.hlsl"));

	blurEffect.SetMainTexture(postEffect.GetResourceHeap());

	depthOfFieldEffect.SetMainTexture(postEffect.GetResourceHeap());
	depthOfFieldEffect.SetBlurTexture(blurEffect.GetResourceHeap());

}
void app::Application::InitExternalLibrary() {

	dxtk::ManagerDXTK::GetInstance()->Init();

	imguiDX12::ManagerImGui::GetInstance()->Init();
}
void app::Application::UnInitExternalLibrary() {

	dxtk::ManagerDXTK::GetInstance()->UnInit();

	imguiDX12::ManagerImGui::GetInstance()->UnInit();

	Resource::GetInstance()->AllDelete();

}
void app::Application::UnInit() {

	lowApp::LowApplication::GetInstance()->UnInit();

}
void app::Application::Load(Scene* _scene) {

	load = true;
	sceneOld = scene;
	if (sceneOld != nullptr) {
		sceneOld->UnLoad();
	}
	scene = _scene;

	scene->Init();

	scene->LoadFrontLoad();

	std::thread loadLoopThread(&app::Application::LoadLoop,this);

	std::thread loadSceneThread(&app::Application::LoadScene,this);

	loadLoopThread.join();
	loadSceneThread.join();
}



void app::Application::Loop() {
	auto lowApp = LowApplication::GetInstance();
	
	MSG msg = {};
	while (true) {

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		lowApp->DrawStart();

		lowApp->DrawOnRenderTarget(postEffect.GetRenderTargetHeap().Get());

		if(lowApp::LowApplication::GetInstance()->GetDebugMode()) {
			imguiDX12::ManagerImGui::GetInstance()->DrawStart();//ImGUI���[�v������
		}
		UpdateInput();//���͏����X�V	

		scene->StartUpdata();

		scene->Updata();

		scene->Draw();
		
		scene->EndUpdata();

		//��ʊE�[�x�p�u���[�e�N�X�`��
		if (depthOfField) {
			//�|�X�g�G�t�F�N�g�Ƀu���[��������RTV�ɕύX
			lowApp->DrawOnRenderTargetNoDepth(blurEffect.GetRenderTargetHeap().Get());

			blurEffect.Draw();
		}
		lowApp->DrawOnScreen();

		//��ʊE�[�x�Ή��`��
		if (depthOfField) {
			depthOfFieldEffect.Draw();
		}
		else {
			postEffect.Draw();
		}

		if (lowApp::LowApplication::GetInstance()->GetDebugMode()) {
			scene->DrawObjectList();
			imguiDX12::ManagerImGui::GetInstance()->DrawFinish();//ImGUI���[�v������
		}
		lowApp->DrawFinish();
		dxtk::ManagerDXTK::GetInstance()->DrawFinish();
	}
}

//�X���b�h�p���[�v
void app::Application::LoadLoop() {

	auto lowApp = LowApplication::GetInstance();
	MSG msg = {};
	while (true) {

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//�����A�v���P�[�V�������I�����Ď���message��WM_QUIT�ɂȂ�
		if (msg.message == WM_QUIT) {
			break;
		}
		lowApp->DrawStart();

		lowApp->DrawOnRenderTarget(postEffect.GetRenderTargetHeap().Get());

		UpdateInput();//���͏����X�V	

		scene->StartUpdata();

		scene->LoadInUpdata();
		scene->LoadInDraw();

		scene->EndUpdata();

		if (depthOfField) {
			lowApp->DrawOnRenderTargetNoDepth(blurEffect.GetRenderTargetHeap().Get());

			blurEffect.Draw();
		}

		lowApp->DrawOnScreen();

		if (depthOfField) {
			depthOfFieldEffect.Draw();
		}
		else {
			postEffect.Draw();
		}

		lowApp->DrawFinish();

		dxtk::ManagerDXTK::GetInstance()->DrawFinish();
	
		if (load == false) {
			break;
		}
	}
}
void app:: Application::LoadScene() {
	scene->Load();//�p���������[�h
	load = false;
}

