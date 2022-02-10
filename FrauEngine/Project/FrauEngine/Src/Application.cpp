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

	//シーンをまたいだボタンを連打しないためにフレーム数をずらす
	mouse->left = 2;
	mouse->right = 2;

	//シーン名を今と前のシーン名を保存しておく
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
			frauEngine::ManagerImGui::GetInstance()->DrawStart();//ImGUIループ枚処理
		}
		UpdateInput();//入力処理更新	

		Time::GetInstance()->Update();//簡易時間加算用クラスの処理

		scene->StartUpdate();

		scene->Update();

		scene->Draw();
		
		scene->EndUpdate();


		//被写界深度用ブラーテクスチャ
		if (depthOfField) {
			//ポストエフェクトにブラーをかけるRTVに変更
			lowApp->DrawOnRenderTargetNoDepth(blurEffect.GetRenderTargetHeap().Get());

			blurEffect.Draw();
		}


		lowApp->DrawOnScreen();

		//被写界深度対応描画
		if (depthOfField) {
			depthOfFieldEffect.Draw();
		}
		else {
			postEffect.Draw();
		}
		//ポストエフェクトなしのシーンからの描画
		scene->DrawNonePostEffect();


		if (LowApplication::GetInstance()->GetDebugMode()) {
			scene->DrawObjectList();
			ManagerImGui::GetInstance()->DrawFinish();//ImGUIループ枚処理
		}
		lowApp->DrawFinish();
		frauEngine::ManagerDXTK::GetInstance()->DrawFinish();

		//シーン移動
		if (scene->loadScene) {
			Load(scene->nextScene);
			scene->loadScene = false;
		}
		//終了処理
		if (scene->gameEnd) {
			break;
		}
	}
}

//スレッド用ループ
void frauEngine::Application::LoadLoop() {

	auto lowApp = LowApplication::GetInstance();
	MSG msg = {};
	while (true) {

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//もうアプリケーションが終わるって時にmessageがWM_QUITになる
		if (msg.message == WM_QUIT) {
			break;
		}
		lowApp->DrawStart();

		lowApp->DrawOnRenderTarget(postEffect.GetRenderTargetHeap().Get());

		UpdateInput();//入力処理更新	

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
		scene->Load();//継承したロード
		
	}
	loadTime = 1;
	loaded = true;
}


