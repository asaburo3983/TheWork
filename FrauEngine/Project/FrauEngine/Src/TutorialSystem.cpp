#include "TutorialSystem.h"


void TutorialSystem::Initialize() {
	auto resource = Resource::GetInstance();

	//チュートリアル文の読み込み
	csv[0].LoadCSV("Data/Tutorial/0.csv");
	csv[1].LoadCSV("Data/Tutorial/1.csv");
	csv[2].LoadCSV("Data/Tutorial/2.csv");
	csv[3].LoadCSV("Data/Tutorial/3.csv");
	
	//チュートリアル用のフレームを設定
	frame.SetResource(resource->Image("TutorialFrame.png"));

	//チュートリアル用の挿絵を設定
	for (int i = 0; i < TUTORIAL_MAX; i++) {
		int pageMax = csv[i].GetRowMax();
		illust[i].resize(pageMax);
	}
	illust[0][0].SetResource(resource->Image("Tutorial0_0.png"));
	illust[0][1].SetResource(resource->Image("Tutorial0_1.png"));
	illust[1][0].SetResource(resource->Image("Tutorial1_0.png"));
	illust[2][0].SetResource(resource->Image("Tutorial2_0.png"));
	illust[2][1].SetResource(resource->Image("Tutorial2_1.png"));
	illust[3][0].SetResource(resource->Image("Tutorial3_0.png"));
}
void TutorialSystem::SetEnable(bool _enable, int _num) {
	auto novel = NovelSystem::GetInstance();
	auto player = Player::GetInstance();
	if (_enable && used[_num] == false && novel->GetEnable() == false) {
		enable = _enable;
		num = _num;
		page = 1;
		LoadPage(text);
		used[num] = true;

		title = csv[num].GetString(0, 0);

		player->IsMove(false);
		
	}
	else if (_enable == false) {
		enable = _enable;
		num = _num;
		page = 1;
		LoadPage(text);
		end[num] = true;
		player->IsMove(true);
	}
}
void TutorialSystem::LoadPage(string* strs) {
	for (int i = 0; i < ROW_MAX; i++) {
		strs[i] = csv[num].GetString(page, i);
	}
}
void TutorialSystem::TurnPage() {
	MouseInput* mouse = MouseInput::GetInstance();
	//ページめくり処理
	if (mouse->left == 1) {
		//Page変数が加算されない
		LoadPage(textOld);
		page++;

		if (csv[num].GetRowMax() <= page) {
			
			SetEnable(false,num);
		}
		else {
			LoadPage(text);
		}
	}
	if (mouse->right == 1 && page > 1) {
		page--;
		LoadPage(text);
	}
}
void TutorialSystem::Update() {
	
	
	if (enable) {
		if (alpha < 1.0f) {
			alpha += alphaSpeed;
		}
		TurnPage();

	}
	else{
		if (alpha > 0.0f) {
			alpha -= alphaSpeed;
		}
	}

}
void TutorialSystem::Draw() {
	Font* meirio = nullptr;
	meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);
	Font* akazukin = nullptr;
	akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);

	//画像の用意がまだなので後で
	if (enable) {
		Vector2 pos = framePos;
		pos.Y += 25;
		illust[num][page - 1].SetAll(pos, Vector2(1, 1), 0, alpha);
		illust[num][page - 1].Draw();
	}
	frame.SetAll(framePos, Vector2(1, 1), 0, alpha);
	frame.Draw();

	//テキスト描画
	
	Color strColor(63.0f / 256.0f, 41.0f / 256.0f, 20.0f / 256.0f, alpha);

	Vector2 titlePos = frame.GetPosLerp(Vector2(0.475, 0.25));
	akazukin->DrawString(title, titlePos, 2.5, strColor, true);

	Vector2 strPos[3];
	strPos[0].X = 530;
	for (int i = 0; i < ROW_MAX; i++) {
		float strPosDistY = 50;
		strPos[i].X = 630;
		strPos[i].Y = 730 + strPosDistY * i;
	}
	float strSize = 1.5f;
	
	
	if (enable) {
		for (int i = 0; i < ROW_MAX; i++) {
			meirio->DrawString(text[i], strPos[i], strSize, strColor);
		}
	}
	else {
		for (int i = 0; i < ROW_MAX; i++) {
			meirio->DrawString(textOld[i], strPos[i], strSize, strColor);
		}
	}

}