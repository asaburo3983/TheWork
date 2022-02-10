#define _CRT_SECURE_NO_WARNINGS

#include "MainGame.h"
#include "Game.h"
void MainGameLoad() {
	//画像読み込み
	GameImage* im = GameImage::GetInstance();
	GameSound* so = GameSound::GetInstance();
	GameStatic* gs = GameStatic::GetInstance();

	so->BGM[0].Load("Data/Sound/Title.mp3");
	so->BGM[1].Load("Data/Sound/stage0.mp3");
	so->BGM[2].Load("Data/Sound/Stage1.mp3");
	so->BGM[3].Load("Data/Sound/Stage2.mp3");
	so->BGM[4].Load("Data/Sound/Stage3.mp3");
	
	for (int i = 0; i < 5; i++) {
		so->BGM->Volume(0.5f);
	}
	so->SE[0].Load("Data/Sound/Enter.mp3");
	so->SE[1].Load("Data/Sound/FootSteps.mp3");
	so->SE[2].Load("Data/Sound/Respawn.mp3");
	so->SE[3].Load("Data/Sound/noize.mp3");
	so->SE[4].Load("Data/Sound/switch.mp3");
	so->SE[5].Load("Data/Sound/TitleNoize.mp3");
	so->SE[6].Load("Data/Sound/GetKey.mp3");
	so->SE[7].Load("Data/Sound/CE.mp3");

	for (int i = 0; i < 8; i++) {
		so->SE->Volume(0.5f);
	}

	im->title[0].Load("Data/Title/MA.png");
	im->title[1].Load("Data/Title/DO.png");
	im->title[2].Load("Data/Title/DE.png");
	im->title[3].Load("Data/Title/RU.png");
	im->title[4].Load("Data/Title/ClickStartText.png");
	im->title[5].Load("Data/Title/Title.png");
	im->title[6].Load("Data/Title/teamName.png");



	im->staticIm[0].Load("Data/static/WindowFrame.png");
	im->staticIm[1].Load("Data/static/WindowFrame_Active.png");
	im->staticIm[2].Load("Data/static/noize.png");
	im->staticIm[3].Load("Data/static/jumpw_0.png");
	im->staticIm[4].Load("Data/static/jumpw_1.png");
	im->staticIm[5].Load("Data/static/movead_0.png");
	im->staticIm[6].Load("Data/static/movead_1.png");
	im->staticIm[7].Load("Data/static/textBase.png");
	im->staticIm[8].Load("Data/static/keyBase.png");

	im->credit[0].Load("Data/Credit/Back.png");
	im->credit[1].Load("Data/Credit/Back1.png");

	im->stageSelect[0].Load("Data/StageSelect/Back0.png");
	im->stageSelect[1].Load("Data/StageSelect/Back1.png");
	im->stageSelect[2].Load("Data/StageSelect/Back2.png");
	im->stageSelect[3].Load("Data/StageSelect/Back3.png");
	im->stageSelect[4].Load("Data/StageSelect/Back4.png");
	im->stageSelect[5].Load("Data/StageSelect/Back5.png");
	im->stageSelect[6].Load("Data/StageSelect/Back6.png");
	im->stageSelect[7].Load("Data/StageSelect/Back7.png");
	im->stageSelect[8].Load("Data/StageSelect/SS.png");

	for (int i = 0; i < 11; i++) {
		char imagePath[100];
		sprintf(imagePath, "Data/Cursor/idou_%d.png", i);
		im->cursorIm[i].Load(imagePath);
	}


	for (int i = 0; i < 60; i++) {
		char imagePath[100];
		sprintf(imagePath, "Data/Effect/tatumaki_%d.png", i);
		im->effect[i].Load(imagePath);
	}

	Player* pl = Player::GetInstance();

	int* animeNumOnNumChangeFrame;
	//アニメフォルダの数を調べる
	while (true) {
		char imagePath[50];
		sprintf(imagePath, "Data/Player/%d", pl->animeNumMax);

		if (!PathFileExists(imagePath))break;

		pl->animeNumMax++;
	}
	pl->animeNumOnNum = new int[pl->animeNumMax];
	pl->animeNumOnNumChangeFrame = new int[pl->animeNumMax];
	//アニメの画像数を調べる
	for (int i = 0; i < pl->animeNumMax; i++) {
		pl->animeNumOnNum[i] = 0;
		while (true) {
			char imagePath[50];
			sprintf(imagePath, "Data/Player/%d/%d.png", i, pl->animeNumOnNum[i]);

			if (!PathFileExists(imagePath))break;

			pl->animeNumOnNum[i]++;
		}
	}

	im->player = new Image*[pl->animeNumMax];
	//実際にロードする
	for (int i = 0; i < pl->animeNumMax; i++) {
		im->player[i] = new Image[pl->animeNumOnNum[i]];
		for (int h = 0; h < pl->animeNumOnNum[i]; h++) {
			char imagePath[50];
			sprintf(imagePath, "Data/Player/%d/%d.png", i, h);
			im->player[i][h].Load(imagePath);
		}
		pl->animeNumOnNumChangeFrame[i] = 5;//とりあえず５Fでアニメが変わるようにする
		switch (i) {
		case 3:
			pl->animeNumOnNumChangeFrame[i] = 10;
			break;
		}
	}

	int mapTipNum = 0;//マップチップの個数になる
	//ファイルがあるか確認してロードする
	while (true) {
		char imagePath[50];
		sprintf(imagePath, "Data/MapTip/%d.png", mapTipNum);

		if (!PathFileExists(imagePath))break;

		mapTipNum++;
	}
	im->mapTipIm = new Image[mapTipNum];
	for (int i = 0; i < mapTipNum; i++) {
		char imagePath[50];
		sprintf(imagePath, "Data/MapTip/%d.png", i);
		im->mapTipIm[i].Load(imagePath);
	}

	TitleLoad();

}


void MainGameUpdata() {
	GameSound* so = GameSound::GetInstance();
	GameStatic* gs = GameStatic::GetInstance();
	gs->Updata();

	if (gs->MOUSE->left == 1) {
		so->SE[0].Play();
	}
	switch (gs->scene) {
	case TITLE:	
		//so->BGM[0].Play();
		TitleUpdata();

		break;
	case STAGESELECT:

		so->BGM[0].Play();
		StageSelectUpdata();

		break;
	case GAME:
		so->BGM[0].Stop();
		GameUpdata();
		break;
	case CREDIT:
		so->BGM[0].Play();
		CreditUpdata();
		break;
	}

}


void MainGameDraw() {
	GameStatic* gs = GameStatic::GetInstance();

	switch (gs->scene) {
	case TITLE:
		TitleDraw();
		break;
	case STAGESELECT:
		
		StageSelectDraw();
		break;
	case GAME:
		GameDraw();
		break;
	case CREDIT:
		CreditDraw();
		break;
	}

}