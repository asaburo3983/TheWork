#include "GameManager.h"
#include "Player12.h"
namespace BattlePlayerVariable {

	Player1* player1;
	Player2* player2;
	Stage* stage;
	int backCount = 0;//背景スクロール用
	int backCountSpeed = 5;
}
using namespace BattlePlayerVariable;
void GameManager::Battle_PlayerLoad() {
	player1 = Player1::GetInstance();
	player2 = Player2::GetInstance();
	stage = Stage::GetInstance();

	player1->Reset();
	player2->Reset();
	stage->Reset(0);
}
void GameManager::Battle_PlayerUpdata() {
	backCount += backCountSpeed;
	player1->Updata();
	player2->Updata();
}
void GameManager::Battle_PlayerDraw() {

	

	DirectGraphics::GetInstance()->SetLight(Vector3(0, 15, 5), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
//影の描画/////////////////////////////////////////////////
//ステージの描画
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			MODEL_DATA md;
			md.scale = Vector3(0.35, 0.35, 0.35);
			md.pos.X = (x - 4) * 0.66;
			md.pos.Z = (y - 4) * 0.66;

			gm->playerAttackEffect_Red->RenderShadowMap(&md);
		}
	}
	player1->DrawShadow();
	player2->DrawShadow();
	int damageBlur = 0;
	DirectGraphics::GetInstance()->SetCamera(Vector3(0 + damageBlur, 6, -3 * 2.5 + damageBlur ), Vector3(0, 1, 0 ), Vector3(0, 1, 0));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

	//ゲームの背景(動く)
	int gameBackX = 960 - backCount % 1920;
	int gameBackY = 540 + backCount % 1080;


	gi->battle[2].Draw(gameBackX, gameBackY - 1080);
	gi->battle[2].Draw(gameBackX, gameBackY);
	gi->battle[2].Draw(gameBackX + 1920, gameBackY);
	gi->battle[2].Draw(gameBackX + 1920, gameBackY-1080);

	//ステージの描画
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			MODEL_DATA md;
			md.scale = Vector3(0.35, 0.35, 0.35);
			md.pos.X = (x - 4) * 0.66;
			md.pos.Z = (y - 4) * 0.66;
			//md.multiplyColor.w = 0.5;
			gm->playerAttackEffect_Red->RenderShadowModel(&md);
		}
	}
	player1->Draw();
	player2->Draw();

	//HPの表示
	for (int h = 0; h < stage->playerWin[0]; h++) {
		gi->barIcon.Draw(480 + h * 150, 80);
	}
	for (int h = 0; h < stage->playerWin[1]; h++) {
		gi->barIcon2.Draw(960+480 - h * 150, 80);
	}
}