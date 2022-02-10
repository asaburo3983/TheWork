#include "GameManager.h"


namespace VS_CPUVariable {
	class Player {
	public:
		MODEL_DATA MD;
		Pos2 pos;

		int power;
		int speed;
		//bool move[3][3] = {
		//	{false,true,false},

		//}

	};
	Player player[2];

	//シーン移動系
	int operationLockCount = 120;
	const int operationLockCountMax = 120;//別のシーンに行くまでのフレーム
	
	//ステージの情報
	float panelDist = 100.0f;
	MODEL_DATA stagePanel_MD[5][5];

	//ゲーム情報
	int playerTurn = 0;


}
using namespace VS_CPUVariable;

void GameManager::VS_CPULoad() {
	operationLockCount = 120;
	//ステージ位置配置
	for (int x = 0; x < 5; x++) {
		for (int z = 0; z < 5; z++) {
			stagePanel_MD[x][z].pos = Vector3((x-2)*panelDist*2, 0, (z-2)*panelDist*2);
			stagePanel_MD[x][z].degree = Vector3(-90, 0, 0);
			stagePanel_MD[x][z].scale = Vector3(100, 100, 10);
		}
	}

	CAMERA* cam = CAMERA::GetInstance();
	cam->pos = Vector3(0, 1000, -1300);
	cam->angle = Vector3(-45, 0, 0);
	cam->focs = Vector3(0, 0, 0);

	player[0].pos.p[0] = 2;
	player[0].pos.p[1] = 4;
	player[0].MD.pos = Vector3((player[0].pos.p[0] - 2) * panelDist * 2, 20, -(player[0].pos.p[1] - 2) * panelDist * 2);
	player[0].MD.degree = Vector3(-90, 0, 0);
	player[0].MD.scale = Vector3(80, 80, 10);

	player[1].pos.p[0] = 2;
	player[1].pos.p[1] = 0;
	player[1].MD.pos = Vector3((player[1].pos.p[0] - 2) * panelDist * 2, 20, -(player[1].pos.p[1] - 2) * panelDist * 2);
	player[1].MD.degree = Vector3(-90, 0, 0);
	player[1].MD.scale = Vector3(80, 80, 10);


}
void GameManager::VS_CPUUpdata() {
	CAMERA* cam = CAMERA::GetInstance();

	player[0].MD.degree.Y++;
	player[1].MD.degree.Y--;

}

void GameManager::VS_CPUDraw() {

	CAMERA* cam = CAMERA::GetInstance();
	DirectGraphics::GetInstance()->SetLight(Vector3(0, 1, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), false);
	

	cam->Debug(5);

	cam->Updata(false);
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 10000.0f, true);

	//背景の表示
	gi->white.Draw(0, 0, 0, 1921);

	//for (int i = 0; i < 6; i++) {
	//	gm->dicePanel[4]->RenderModel(&panel_MD[i]);
	//}
	//gm->player[0]->RenderModel(&player_MD);


	for (int x = 0; x < 5; x++) {
		for (int z = 0; z < 5; z++) {
			gm->dicePanel[4]->RenderModel(&stagePanel_MD[x][z]);
		}
	}
	gm->player[0]->RenderModel(&player[0].MD);
	gm->player[1]->RenderModel(&player[1].MD);

	//フェード表示
	if (operationLockCount != 0) {
	//	gi->white.Draw(0, 0, 0, 1920, 0 + (float)operationLockCount / (float)operationLockCountMax, 1);
	}
}