#include "MainGame.h"

int cTime = 0;
float fade = 0.0f;
void CreditLoad() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	cTime = 0;
	fade = 0;
}
void CreditUpdata() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	cTime++;
}
void CreditDraw() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	im->credit[0].Draw(960, 540, 0, 2.5);

	im->credit[1].Draw(760*2, 540-cTime%1080, 0, 2);
	im->credit[1].Draw(760*2, 540+1080-cTime%1080, 0, 2);

	// 5~8番
	int num = 120;
	int iNum = 0;
	if (cTime % num < num/6) {
		iNum = 5;
	}	
	else if (cTime % num < num /5 ) {
		iNum = 6;
	}	
	else if (cTime % num<num/4) {
		iNum = 7;
	}	
	else if (cTime % num<num/3) {
		iNum = 8;
	}
	else if (cTime % num < num / 2) {
		iNum = 7;
	}
	else if (cTime % num < num / 1) {
		iNum = 6;
	}
	float a = GetSinWave(cTime) * 300;;
	im->player[2][iNum].Draw(780 * 2, 540+a, 0, 2);


	int strX = 150;
	int strY = 1500;
	strY -= cTime;
	int strDist=200;
	DrawString(L"プログラム　小野田凌晴", strX, strY+strDist, StrColor{ 1,1,1,1 }, 3);
	DrawString(L"動画作成　高柳涼介",			strX, strY+strDist*2, StrColor{ 1,1,1,1 }, 3);
	DrawString(L"イラスト　浅野裕也",			strX, strY+strDist*3, StrColor{ 1,1,1,1 }, 3);
	DrawString(L"音楽　魔王魂　様",				strX, strY+strDist*4, StrColor{ 1,1,1,1 }, 3);

	DrawString(L"遊んでくれてありがとう",				strX, strY+strDist*8, StrColor{ 1,1,1,1 }, 3);

	if (strY - cTime + strDist * 8 < -540*4) {
		fade += 0.01f;
		im->credit[0].Draw(960, 540, 0, 2.5,fade);

		if (fade >= 2) {
			TitleLoad();
			gs->scene = TITLE;

			SetClear();
		}
	}

}