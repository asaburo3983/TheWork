#define _CRT_SECURE_NO_WARNINGS
#include "EditorScene.h"

#include "CameraObject.h"
#include "Light.h"
#include "ImGuiDX12.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void EditorScene::LoadFrontLoad() {

}
//ロード中の処理
void EditorScene::LoadInUpdata() {
}
void EditorScene::LoadInDraw() {

}


//実際のロード処理
obj::ModelObject model;
obj::CameraObject camera;
light::Lights* light1;
void EditorScene::Load() {
	//resource->LoadShader(shader::VS, "Data/Shader/VertexShader.hlsl");
	//resource->LoadShader(shader::PS, "Data/Shader/PixelShader.hlsl");

	//resource->LoadIm("Test.png");
	//resource->LoadSound("Test.mp3");

	////先に３Dシステムの完成を進める
	////エフェクトとIMGUIのテストは後ほど行う
	//resource->LoadModel("Test.fbx", "Data/");
	//model.Initialize(resource->Model("Test.fbx"),
	//	resource->Shader("Data/Shader/VertexShader.hlsl"),
	//	resource->Shader("Data/Shader/PixelShader.hlsl"));


	camera.SetPos(Vector3(0, 0, 12));
	camera.SetTarget(Vector3(0, 0, 0));
	camera.SetUp(Vector3(0, 1, 0));
	camera.SetFov(45);

	light::Lights::GetInstance()->SetDirectionalLight(Vector3(-5, 8, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, -1, 1), Vector3(0.5, 0.5, 0.5));
//	light1 = light::DirectionalLight::GetInstance();

	//light1->SetLight(Vector3(0, 0, 12), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, -1, 1), Vector3(0.5, 0.5, 0.5));


	SetObjectList(&materialLinker.modelObj, "Main");
	SetObjectList(&backObject.modelObj, "Back");

//	objectList.push_back(&materialLinker.modelObj);

//	objectList.push_back(&backObject.modelObj);

	
}
void EditorScene::UnLoad() {
}

void EditorScene::Updata() {

	materialLinker.Manager();
	backObject.Manager();
}


void EditorScene::Draw() {
	MouseInput* ms = MouseInput::GetInstance();

	auto dLight = light::Lights::GetInstance();
	camera.SetPos(Vector3(0, 0, -12));
	camera.SetTarget(Vector3(0, 0, 0));
	camera.SetUp(Vector3(0, 1, 0));
	camera.SetFov(45);
	camera.Draw();

	auto lowApp = lowApp::LowApplication::GetInstance();

	dLight->SetAmbientLight(0.7f);
	/*dLight->SetPointLight(0, Vector3(5, 0, 0), 5, Vector3(1, 0, 1));
	dLight->SetPointLight(1, Vector3(5, 0, 0), 5, Vector3(1, 0, 1));
	dLight->SetPointLight(2, Vector3(0, 3, 0), 8, Vector3(0, 1, 0));
	dLight->SetPointLight(3, Vector3(0, -3, 0), 8, Vector3(0, 0, 1));*/

	//dLight->SetSpotLight(0, Vector3(0, 3, 0), 8, Vector3(0, -1, 0), 30, Vector3(1, 0, 0));
	//dLight->SetSpotLight(1, Vector3(0, -3, 0), 8, Vector3(0, 1, 0), 30, Vector3(1, 0, 1));
	//dLight->SetSpotLight(2, Vector3(5, 0, 0), 8, Vector3(-1, 0, 0), 30, Vector3(1, 1, 0));
	//dLight->SetSpotLight(3, Vector3(-5, 0, 0), 8, Vector3(1, 0, 0), 30, Vector3(0, 0, 1));

	lowApp->DrawOnDepth(dLight->depthHeap, 1024);
	materialLinker.DrawShadow();

	//backObject.DrawShadow();

	lowApp->DrawOnRenderTarget(app::Application::GetInstance()->GetPostEffectRenderTarget());

	materialLinker.DrawModel();

	//backObject.DrawModel();

	//被写界深度適応
	//app::Application::GetInstance()->SetDepthOfField(true, ms->x, ms->y);


}