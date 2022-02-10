#pragma warning(disable:C26812)
#pragma warning(disable:C4067)

#pragma comment(lib, "libfbxsdk-mt.lib")
#pragma comment(lib, "zlib-mt.lib")
#pragma comment(lib, "libxml2-mt.lib")

#include "DirectX11Lib.h"
#include "GameManager.h"


using namespace DXTK11LIB;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{

	int wx = GetSystemMetrics(SM_CXSCREEN);
	int wy = GetSystemMetrics(SM_CYSCREEN);

	//SetAntialias(false);//←意図的にアンチエイリアスをオフにできる
	AllInit("Death_T", wx, wy,true, hInstance);
	
	SetDefaultImageExtend((float)wx / 1920.0);

	GameManager* gm = GameManager::GetInstance();
	gm->AllLoad();
	
	gm->scene = TITLE;


	//gm->bgmVolume = 0;
	//gm->seVolume = 0;
	//音量の設定
	for (int i = 0; i < 12; i++) {
		gm->gs->BGM[i].Volume(gm->bgmVolume);
	}
	gm->gs->MovePlayer.Volume(gm->seVolume);
	gm->gs->SetBlock.Volume(gm->seVolume);
	gm->gs->Enter.Volume(gm->seVolume);
	gm->gs->Dead.Volume(gm->seVolume);
	gm->gs->MoveCursor.Volume(gm->seVolume);


	gm->gs->Attack.Volume(gm->seVolume);
	gm->gs->Boss1FrontAttack.Volume(gm->seVolume);
	gm->gs->Boss1Tackle.Volume(gm->seVolume);
	gm->gs->Boss1Avalanche.Volume(gm->seVolume);
	gm->gs->Boss2Laser.Volume(gm->seVolume);
	gm->gs->Boss2Homing.Volume(gm->seVolume);
	gm->gs->Boss3Attack.Volume(gm->seVolume);
	gm->gs->Boss3Laser.Volume(gm->seVolume);
	gm->gs->Boss3Thunder.Volume(gm->seVolume);

	gm->gs->EnemyDead.Volume(gm->seVolume);
	gm->gs->PlayerDamage.Volume(gm->seVolume);
	gm->gs->PlayerDead.Volume(gm->seVolume);
	gm->gs->ItemGet.Volume(gm->seVolume);
	gm->gs->Bom.Volume(gm->seVolume);

	while (true)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{

			UpdateInput();//入力処理更新	
			DirectGraphics::GetInstance()->StartRendering();

				//BGMを流す
				if (gm->scene < BATTLE) {
					gm->gs->BGM[gm->scene].Play();
				}

				if (gm->scene != gm->sceneOld) {
					gm->sceneBack = gm->sceneOld;
					switch (gm->scene) {
					case TITLE:
						gm->TitleLoad();
						break;
					case SELECT:
						gm->SelectLoad();
						break;
					case OPTION:
						gm->OptionLoad();
						break;

						//廃止してバトルにする
					case TOWER:
						gm->Battle_PlayerLoad();
						break;
					case TOWER_CLEAR:
						gm->ClearLoad();
						break;


					case BATTLE:
						gm->BattleLoad(0);
						break;
					case BATTLE_CLEAR:
						gm->Battle_ClearLoad();
						break;
					case CLEDIT:
						gm->CleditLoad();
						break;
					}

					gm->sceneOld = gm->scene;
				}

				switch (gm->scene) {
				case TITLE:

					gm->TitleUpdata();
					gm->TitleDraw();
					break;
				case SELECT:
					gm->SelectUpdata();
					gm->SelectDraw();

					break;
				case OPTION:
					gm->OptionUpdata();
					gm->OptionDraw();
					break;


				case TOWER:
					gm->Battle_PlayerUpdata();
					gm->Battle_PlayerDraw();

					break;
				case TOWER_CLEAR:
					gm->ClearUpdata();
					gm->ClearDraw();
					break;


				case BATTLE:
					gm->BattleUpdata();
					gm->BattleDraw();

					break;
				case BATTLE_CLEAR:
					gm->Battle_ClearUpdata();
					gm->Battle_ClearDraw();

					break;
				case CLEDIT:
					
					gm->CleditUpdata();
					gm->CleditDraw();
					break;
				}
				//DrawFPS();
			DirectGraphics::GetInstance()->FinishRendering();//描画する毎回の処理
			if (gm->gameEnd || gm->KEY->key[DIK_ESCAPE]) {
				if (gm->scene != BATTLE) {
					break;
				}
			}
		}
	}

	UninitInput();
	DirectGraphics::GetInstance()->Release();

	return 0;
}
