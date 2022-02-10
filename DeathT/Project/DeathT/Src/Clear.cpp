#include "GameManager.h"

namespace ClearVariable {
	int titleCount = 0;
	int feedCount = 0;

}
using namespace ClearVariable;

void GameManager::ClearLoad() {
	 titleCount = 0;
	 feedCount = 0;
}

void GameManager::ClearUpdata() {
	if (titleCount < 360) {
		titleCount++;
	}
	else {
		titleCount = 0;
	}
	if (feedCount == 0 && (KEY->key[DIK_A] == 1 ||
		KEY->key[DIK_W] == 1 ||
		KEY->key[DIK_S] == 1 ||
		KEY->key[DIK_D] == 1)) {
		feedCount = 1;
		gs->Enter.Play();
	}
	if (feedCount) {
		feedCount++;
		if (feedCount >= 100) {
			feedCount = 0;
			gs->BGM[scene].Stop();
			scene = SELECT;
			titleCount = 0;
			feedCount = 0;
		}
	}
}

void GameManager::ClearDraw() {
	gi->clearBack.Draw(960, 540);
	DirectGraphics::GetInstance()->SetLight(Vector3(0, -4, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
	DirectGraphics::GetInstance()->SetCamera(Vector3(0, -2, -4), Vector3(0, -1, 0), Vector3(0, 0, 0.1));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

	MODEL_DATA md;
	md.pos = Vector3(0.0f, -0.2f, 0.0f);
	md.scale = Vector3(0.7f, 0.7f, 0.7f);
	md.degree = Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount);
	gm->Player->RenderModel(&md);
	md.pos = Vector3(-1.7f, -0.2f, 0.0f);
	md.scale = Vector3(0.4f, 0.4f, 0.4f);
	md.degree = Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount);
	gm->Player->RenderModel(&md);
	md.pos = Vector3(+1.7f, -0.2f, 0.0f);
	md.scale = Vector3(0.4f, 0.4f, 0.4f);
	md.degree = Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount);
	gm->Player->RenderModel(&md);

	gi->feedBlack.Draw(960, 540, 0, 1, (float)feedCount / 100.0f);

}


