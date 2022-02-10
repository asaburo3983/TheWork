#include "Application.h"

//vector str;
using namespace frauEngine;




void frauEngine::Application::Init(const char* _windowName, int _windowSizeX, int _windowSizeY, bool _fullscreen) {

	frauEngine::LowApplication::GetInstance()->SetDebugMode(true);

	frauEngine::LowApplication::GetInstance()->Init(_windowName, _windowSizeX, _windowSizeY, _fullscreen);

	InitExternalLibrary();

	auto resource = Resource::GetInstance();
	resource->LoadShader(frauEngine::ShaderType::VS, "Data/Shader/VertexShader_PostEffect.hlsl");
	resource->LoadShader(frauEngine::ShaderType::PS, "Data/Shader/PixelShader_PostEffect.hlsl");

	resource->LoadShader(frauEngine::ShaderType::PS, "Data/Shader/PixelShader_Blur.hlsl");
	resource->LoadShader(frauEngine::ShaderType::PS, "Data/Shader/PixelShader_DepthOfField.hlsl");

	postEffect.Create(
		resource->Shader("VertexShader_PostEffect.hlsl"),
		resource->Shader("PixelShader_PostEffect.hlsl"));
	blurEffect.Create(
		resource->Shader("VertexShader_PostEffect.hlsl"),
		resource->Shader("PixelShader_Blur.hlsl"));

	depthOfFieldEffect.Create(
		resource->Shader("VertexShader_PostEffect.hlsl"),
		resource->Shader("PixelShader_DepthOfField.hlsl"));

	blurEffect.SetMainTexture(postEffect.GetResourceHeap());

	depthOfFieldEffect.SetMainTexture(postEffect.GetResourceHeap());
	depthOfFieldEffect.SetBlurTexture(blurEffect.GetResourceHeap());

}
void frauEngine::Application::InitExternalLibrary() {

	frauEngine::ManagerDXTK::GetInstance()->Init();

	frauEngine::ManagerImGui::GetInstance()->Init();
}
void frauEngine::Application::UnInitExternalLibrary() {

	frauEngine::ManagerDXTK::GetInstance()->UnInit();

	frauEngine::ManagerImGui::GetInstance()->UnInit();

	Resource::GetInstance()->AllDelete();

}
void frauEngine::Application::UnInit() {

	frauEngine::LowApplication::GetInstance()->UnInit();

}
void Application::SetSceneList(std::string _str, Scene* _scene) {
	sceneList[_str] = _scene;
}
void Application::Load(string _nextScene) {

	auto mouse = MouseInput::GetInstance();

	//�V�[�����܂������{�^����A�ł��Ȃ����߂Ƀt���[���������炷
	mouse->left = 2;
	mouse->right = 2;

	//�V�[���������ƑO�̃V�[������ۑ����Ă���
	sceneOldStr = sceneStr;
	sceneStr = _nextScene;


	loadTime = 0;
	loaded = false;

	sceneOld = scene;
	if (sceneOld != nullptr) {
		sceneOld->UnLoad();
	}
	scene = sceneList[_nextScene];

	scene->Init();

	scene->LoadFrontLoad();

	std::thread loadLoopThread(&frauEngine::Application::LoadLoop,this);

	std::thread loadSceneThread(&frauEngine::Application::LoadScene,this);

	loadLoopThread.join();
	loadSceneThread.join();
}



void frauEngine::Application::Loop() {
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

		if(frauEngine::LowApplication::GetInstance()->GetDebugMode()) {
			frauEngine::ManagerImGui::GetInstance()->DrawStart();//ImGUI���[�v������
		}
		UpdateInput();//���͏����X�V	

		Time::GetInstance()->Update();//�ȈՎ��ԉ��Z�p�N���X�̏���

		scene->StartUpdate();

		scene->Update();

		scene->Draw();
		
		scene->EndUpdate();


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
		//�|�X�g�G�t�F�N�g�Ȃ��̃V�[������̕`��
		scene->DrawNonePostEffect();


		if (LowApplication::GetInstance()->GetDebugMode()) {
			scene->DrawObjectList();
			ManagerImGui::GetInstance()->DrawFinish();//ImGUI���[�v������
		}
		lowApp->DrawFinish();
		frauEngine::ManagerDXTK::GetInstance()->DrawFinish();

		//�V�[���ړ�
		if (scene->loadScene) {
			Load(scene->nextScene);
			scene->loadScene = false;
		}
		//�I������
		if (scene->gameEnd) {
			break;
		}
	}
}

//�X���b�h�p���[�v
void frauEngine::Application::LoadLoop() {

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

		scene->StartUpdate();

		scene->LoadInUpdate();
		scene->LoadInDraw();

		scene->EndUpdate();

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

		frauEngine::ManagerDXTK::GetInstance()->DrawFinish();
	
		loadTime++;

		if (loadTime >= loadTimeMaxFrame && loaded == true) {
			break;
		}
	}
}
void frauEngine:: Application::LoadScene() {
	
	if (loadTime == 0) {
		
		if (scene->GetStaticLoad()) {
			scene->StaticLoad();
			scene->SetStaticLoadOFF();
		}
		scene->Load();//�p���������[�h
		
	}
	loadTime = 1;
	loaded = true;
}


