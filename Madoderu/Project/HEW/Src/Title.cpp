#include "MainGame.h"


int ClickStartText_DrawTime = 0;

int startUp = 0;
int noizeTiming = 0;
void TitleLoad() {

	startUp = 0;
	noizeTiming = 0;
	ClickStartText_DrawTime = 0;
}

void TitleUpdata() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	startUp++;
	ClickStartText_DrawTime++;
	if (ClickStartText_DrawTime > 150) {
		ClickStartText_DrawTime = 0;
		noizeTiming = GetRand(15, 150);
	}

	if (gs->MOUSE->left == 1&& startUp>300) {
		gs->scene = STAGESELECT;
		StageSelectLoad();
	}


}

void TitleDraw() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();
	GameSound* so = GameSound::GetInstance();


	//タイトル画像
	const int titleSize = 2200;
	const int titleY = 110;
	int dist = titleSize / titleY;
	for (int y = 0; y < titleY; y++) {
		int plusX=0;
		if (ClickStartText_DrawTime < noizeTiming && noizeTiming -10 < ClickStartText_DrawTime) {
			plusX = GetRand(100, 500)-300;
		
		}
			im->title[5].DrawEX(960 + plusX, 540 + y * 10, 0, 0.5, 0.5, 1, 1, false, false, RECT{ 0,0 + y * dist,2200, dist + y * dist });
		
	}
	if (ClickStartText_DrawTime < noizeTiming && noizeTiming - 10 < ClickStartText_DrawTime) {
		so->SE[5].Play();
	}
	else {
		so->SE[5].Stop();
	}
	//im->title[5].Draw(960, 540, 0, 0.5);
	//クリックしてねテキスト画像
	if (ClickStartText_DrawTime < 90) {
		im->title[4].Draw(960, 540 + 540 / 1.5);
	}


}

