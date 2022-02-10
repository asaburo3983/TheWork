// strictStrings_off.cpp
#include "Application.h"

#include "EditorScene.h"

using namespace DirectX;
using namespace std;

using namespace lowApp;
using namespace app;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	int wx = GetSystemMetrics(SM_CXSCREEN);
	int wy = GetSystemMetrics(SM_CYSCREEN);

	Application::GetInstance()->Init("PBRマテリアル作成ソフト", wx, wy, false);

	EditorScene editorScene;

	Application::GetInstance()->Load(&editorScene);

	Application::GetInstance()->Loop();

	Application::GetInstance()->UnInit();

	LowApplication::GetInstance()->UnInit();

}

