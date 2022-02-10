#include "GameManager.h"
namespace Battle_ClearVariable {
	MODEL_DATA player_MD;
	
	int clearCount = 0;

	int score_timeF=0;
	int score_enemy = 0;

}
using namespace Battle_ClearVariable;


void GameManager::Battle_ClearSetScore(int _timeF, int _deleteEnemy){
	score_timeF = _timeF;
	score_enemy = _deleteEnemy;
}

void GameManager::Battle_ClearLoad() {

}
void GameManager::Battle_ClearUpdata() {
	player_MD.pos = Vector3(1.5f, -0.2f, 0.0f);
	player_MD.degree = Vector3(90.0f, 225.0f + clearCount, 0.0f);
	player_MD.scale = Vector3(0.5f, 0.5f, 0.5f);
	gs->BGM[4].Play(true);

	if (clearCount < 360) {
		clearCount++;
	}
	else {
		clearCount=0;
	}

	if (MOUSE->left==1) {
		gs->Enter.Play();
		gs->BGM[4].Stop();
		scene = CLEDIT;
	}

}
void GameManager::Battle_ClearDraw() {

	gi->stage1Clear.Draw(960, 540);
	DirectGraphics::GetInstance()->SetLight(Vector3(0, -4, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
	gm->Player->RenderShadowMap(&player_MD);
	DirectGraphics::GetInstance()->SetCamera(Vector3(0, -2, -4), Vector3(0, -1, 0), Vector3(0, 0, 0.1));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

	int sec = score_timeF / 60;
	if (sec > 10000 || sec <= 0) {
		sec = 9999;
	}
	gi->numbarFont[sec / 1000 % 10].Draw(1550, 570);
	gi->numbarFont[sec / 100 % 10].Draw(1550+1*95,     570);
	gi->numbarFont[sec / 10  % 10].Draw(1550+2*95, 570);
	gi->numbarFont[sec / 1   % 10].Draw(1550+3*95, 570);
	
	sec = score_enemy;
	if (sec > 10000 || sec <= 0) {
		sec = 9999;
	}

	gi->numbarFont[sec / 1000 % 10].Draw(1550, 770);
	gi->numbarFont[sec / 100 % 10].Draw(1550+95,      770);
	gi->numbarFont[sec / 10 % 10].Draw(1550 + 95*2, 770);
	gi->numbarFont[sec / 1 % 10].Draw(1550 + 95*3,  770);

	gm->Player->RenderShadowModel(&player_MD);



	//gm->Block->RenderModel(&block_MD[0]);
	//gm->Block->RenderModel(&block_MD[1]);

	//gi->feedBlack.Draw(960, 540, 0, 1, (float)feedCount / 100.0f);
}
