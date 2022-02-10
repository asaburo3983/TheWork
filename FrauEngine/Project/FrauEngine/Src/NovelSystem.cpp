#include "NovelSystem.h"
#include "SoundManager.h"
void NovelSystem::Initialize() {
	resource = Resource::GetInstance();

	//UI設定
	textBase.SetResource(resource->Image("Novel_TextBase.png"));
	textBase.SetAll(Vector2(960, 900), Vector2(1, 1), 0, 0);

	auto frameImage = resource->Image("Novel_Frame.png");
	frame[0].SetResource(frameImage);
	frame[0].SetAll(Vector2(0, 1080 - frameImage->GetHeight()), Vector2(1, 1), 0, 0);
	frame[0].SetCenter(false);
	frame[0].SetTurn(true, false);

	frame[1].SetResource(frameImage);
	frame[1].SetAll(Vector2(1920 - frameImage->GetWidth(), 1080 - frameImage->GetHeight()), Vector2(1, 1), 0, 0);
	frame[1].SetCenter(false);

	//立ち絵設定
	chara[0].SetResource(resource->Image("Frau_Normal.png"));
	chara[1].SetResource(resource->Image("Frau_Smile.png"));
	chara[2].SetResource(resource->Image("Frau_Surprise.png"));
	chara[3].SetResource(resource->Image("Frau_Trouble.png"));

	chara[4].SetResource(resource->Image("Christa_Normal.png"));
	chara[5].SetResource(resource->Image("Christa_Smile.png"));
	chara[6].SetResource(resource->Image("Christa_Surprise.png"));
	chara[7].SetResource(resource->Image("Christa_Trouble.png"));

	chara[8].SetResource(resource->Image("Handy_Normal.png"));
	chara[9].SetResource(resource->Image("Handy_Smile.png"));
	chara[10].SetResource(resource->Image("Handy_Surprise.png"));
	chara[11].SetResource(resource->Image("Handy_Trouble.png"));

	autoButton.SetResource(resource->Image("Novel_Button.png"));
	skipButton.SetResource(resource->Image("Novel_Button.png"));

	//シナリオ設定
	scenario.ecpression[""] = -1;
	scenario.ecpression[scenario.FRAU_NORMAL] = 0;
	scenario.ecpression[scenario.FRAU_SMILE] = 1;
	scenario.ecpression[scenario.FRAU_SURPRISE] = 2;
	scenario.ecpression[scenario.FRAU_TROUBLE] = 3;

	scenario.ecpression[scenario.CHRISTA_NORMAL] = 4;
	scenario.ecpression[scenario.CHRISTA_SMILE] = 5;
	scenario.ecpression[scenario.CHRISTA_SURPRISE] = 6;
	scenario.ecpression[scenario.CHRISTA_TROUBLE] = 7;

	scenario.ecpression[scenario.HANDY_NORMAL] = 8;
	scenario.ecpression[scenario.HANDY_SMILE] = 9;
	scenario.ecpression[scenario.HANDY_SURPRISE] = 10;
	scenario.ecpression[scenario.HANDY_TROUBLE] = 11;

	InitializeScenario();


}
void NovelSystem::InitializeScenario() {
	scenario.csv[(int)ScenarioName::PROLOGUE].LoadCSV("Data/Scenario/0.csv");
	scenario.csv[(int)ScenarioName::HANDYSHOP_JOIN].LoadCSV("Data/Scenario/HandyShopJoin.csv");
	scenario.csv[(int)ScenarioName::MAGICSHOP_JOIN].LoadCSV("Data/Scenario/MagicShopJoin.csv");
	
	scenario.csv[(int)ScenarioName::HANDY_EVENT1].LoadCSV("Data/Scenario/HandyEvent1.csv");
	scenario.csv[(int)ScenarioName::HANDY_EVENT2].LoadCSV("Data/Scenario/HandyEvent2.csv");
	scenario.csv[(int)ScenarioName::MAGIC_EVENT1].LoadCSV("Data/Scenario/MagicEvent1.csv");

	scenario.csv[(int)ScenarioName::HAPPY_END].LoadCSV("Data/Scenario/HappyEnd.csv");
	scenario.csv[(int)ScenarioName::BAD_END].LoadCSV("Data/Scenario/BadEnd.csv");

	scenario.leftCharaNumOld = 0;
	scenario.rightCharaNumOld = 0;
	LoadPage();
}
void NovelSystem::SetEnable(bool _enable, ScenarioName _scenario) {

	if (_enable == true && (end[(int)_scenario] == true || start[(int)_scenario] == true)) {
		return;
	}
	enable = _enable;



	auto player = Player::GetInstance();
	player->IsMove(!enable);
	player->IsCollision(!enable);

	CameraWork* cameraWork = CameraWork::GetInstance();
	if (enable == false) {
		cameraWork->SetMoveNum(0);//通常のカメラ位置に戻す
		end[scenario.num] = true;
	}
	if (_scenario != ScenarioName::MAX) {
		scenario.num = (int)_scenario;
	}
	if (_enable == true) {

		autoCount = 0;
		scenario.page = 1;
		start[scenario.num] = true;
		LoadPage();
		charaAlpha[0] = 0;
		charaAlpha[1] = 0;

	}
}

void NovelSystem::LoadPage() {

	//文字列を読み込む
	for (int i = 0; i < (int)ScenarioDataName::MAX; i++) {
		scenario.str[i] = scenario.csv[scenario.num].GetString(scenario.page, i);
	}
	//キャラ画像の番号を読み込む
	scenario.leftCharaNum = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_LEFT]];
	scenario.rightCharaNum = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_RIGHT]];

}
void NovelSystem::TurnPage() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();
	//テキスト送り

	scenario.page++;
	//終了処理
	if (scenario.page >= scenario.csv[scenario.num].GetRowMax()) {
		ScenarioEnd();
	}
	else {
		//前のキャラ番号を保存しておく
		scenario.leftCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_LEFT]];
		scenario.rightCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_RIGHT]];
		if (scenario.leftCharaNumOld < 0)
			scenario.leftCharaNumOld = 0;
		if (scenario.rightCharaNumOld < 0)
			scenario.rightCharaNumOld = 0;
		sound->GetSE(SoundList_SE::TURNPAGE)->Play();
		//ページの読み込み
		LoadPage();

		//文字数でページめくりのオート時間を設定する
		int strLength = 0;
		for (int i = 0; i < 3; i++) {
			int strNum = (int)ScenarioDataName::SCENARIO_1 + i;
			strLength += scenario.str[strNum].length();
		}
		autoCountMax = strLength * autoSpeed;
	}

	autoCount = 0;
}
void NovelSystem::ScenarioEnd() {
	auto sound = SoundManager::GetInstance();

	scenario.page = 1;
	SetEnable(false);

	scenario.leftCharaNum = -1;
	scenario.rightCharaNum = -1;

	scenario.leftCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_LEFT]];
	scenario.rightCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_RIGHT]];

	switch (scenario.num) {
		break;
	case (int)ScenarioName::HAPPY_END:
		sound->GetBGM(SoundList_BGM::HAPPY_END)->Stop();
		break;
	case (int)ScenarioName::BAD_END:
		sound->GetBGM(SoundList_BGM::BAD_END)->Stop();
		break;
	}
}
void NovelSystem::Update() {
	auto sound = SoundManager::GetInstance();
	auto mouse = MouseInput::GetInstance();
	if (enable) {
		if (count < countMax) {
			count++;
		}

		if (mouse->left == 1) {

			TurnPage();
		}
		//オートページめくり機能
		if (autoMode) {
			if (autoCount < autoCountMax) {
				autoCount++;
			}
			else {
				TurnPage();
			}
		}
		switch (scenario.num) {
		case (int)ScenarioName::PROLOGUE:
		case (int)ScenarioName::MAGICSHOP_JOIN:
		case (int)ScenarioName::MAGIC_EVENT1:
			sound->PlayFade(SoundList_BGM::SHINDY_THEME);
			break;
		case (int)ScenarioName::HANDYSHOP_JOIN:
		case (int)ScenarioName::HANDY_EVENT1:
		case (int)ScenarioName::HANDY_EVENT2:
			sound->PlayFade(SoundList_BGM::HANDY_THEME);
			break;
		case (int)ScenarioName::HAPPY_END:
			sound->PlayFade(SoundList_BGM::HAPPY_END);
			break;
			case (int)ScenarioName::BAD_END:
		sound->PlayFade(SoundList_BGM::BAD_END);
			break;
		}

		if (autoButton.Hit(mouse->x, mouse->y)&&mouse->left==1) {
			autoMode = !autoMode;
		}		
		if (skipButton.Hit(mouse->x, mouse->y)&&mouse->left==1) {
			//前のキャラ番号を保存しておく
			scenario.leftCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_LEFT]];
			scenario.rightCharaNumOld = scenario.ecpression[scenario.str[(int)ScenarioDataName::ECPRESSION_RIGHT]];
			if (scenario.leftCharaNumOld < 0)
				scenario.leftCharaNumOld = 0;
			if (scenario.rightCharaNumOld < 0)
				scenario.rightCharaNumOld = 0;
			sound->GetSE(SoundList_SE::TURNPAGE)->Play();
			//ページの読み込み
			LoadPage();

			//文字数でページめくりのオート時間を設定する
			int strLength = 0;
			for (int i = 0; i < 3; i++) {
				int strNum = (int)ScenarioDataName::SCENARIO_1 + i;
				strLength += scenario.str[strNum].length();
			}
			autoCountMax = strLength * autoSpeed;
			ScenarioEnd();
		}
	}
	else {
		if (count >0 ) {
			count--;
		}
		switch (scenario.num) {
		case (int)ScenarioName::PROLOGUE:
		case (int)ScenarioName::MAGICSHOP_JOIN:
		case (int)ScenarioName::MAGIC_EVENT1:
			sound->StopFade(SoundList_BGM::SHINDY_THEME);
			break;
		case (int)ScenarioName::HANDYSHOP_JOIN:
		case (int)ScenarioName::HANDY_EVENT1:
		case (int)ScenarioName::HANDY_EVENT2:
			sound->StopFade(SoundList_BGM::HANDY_THEME);
			break;
		case (int)ScenarioName::HAPPY_END:
			sound->GetBGM(SoundList_BGM::HAPPY_END)->Stop();
			break;
		case (int)ScenarioName::BAD_END:
			sound->GetBGM(SoundList_BGM::BAD_END)->Stop();
			break;
		}
	}

	


	//画像関連
	alphaUI = (float)count / (float)countMax;
	textBase.SetAlpha(alphaUI);
	frame[0].SetAlpha(alphaUI);
	frame[1].SetAlpha(alphaUI);

	frame[0].SetPos(Vector2(0 - lerp(frame[0].GetWidth() / 2, 0, alphaUI), 1080 - frame[0].GetHeight()));
	frame[1].SetPos(Vector2(1920 - frame[1].GetWidth() + lerp(frame[0].GetWidth() / 2, 0, alphaUI), 1080 - frame[1].GetHeight()));
}
void NovelSystem::Draw() {

	DrawChara();

	DrawUI();

	DrawScenarioText();
}
void NovelSystem::DrawChara() {

	if (scenario.leftCharaNum == -1) {
		if(charaAlpha[0]>0)
			charaAlpha[0] -= charaAlphaSpeed;
	}
	else {
		if (charaAlpha[0] < 1)
			charaAlpha[0] += charaAlphaSpeed;
	}

	if (scenario.rightCharaNum == -1) {
		if (charaAlpha[1] > 0)
			charaAlpha[1] -= charaAlphaSpeed;
	}
	else {
		if (charaAlpha[1] < 1)
			charaAlpha[1] += charaAlphaSpeed;
	}


	Vector2 pos[2] = { charaPos[0] ,charaPos[1] };
	pos[0].X = lerp(charaPos[0].X - charaPosDistX, charaPos[0].X, charaAlpha[0]);
	pos[1].X = lerp(charaPos[1].X + charaPosDistX, charaPos[1].X, charaAlpha[1]);
	Vector2 scale = { charaScale,charaScale };
	if (scenario.leftCharaNum != -1) {
		chara[scenario.leftCharaNum].SetAll(pos[0], scale, 0, charaAlpha[0]);
		chara[scenario.leftCharaNum].SetTurn(true, false);
		chara[scenario.leftCharaNum].Draw();
		
	}
	else {
		if (scenario.leftCharaNumOld != -1) {
			chara[scenario.leftCharaNumOld].SetAll(pos[0], scale, 0, charaAlpha[0]);
			chara[scenario.leftCharaNumOld].SetTurn(true, false);
			chara[scenario.leftCharaNumOld].Draw();
		}
	}
	if (scenario.rightCharaNum != -1) {
		chara[scenario.rightCharaNum].SetAll(pos[1], scale, 0, charaAlpha[1]);
		chara[scenario.rightCharaNum].SetTurn(false, false);
		chara[scenario.rightCharaNum].Draw();
		
	}
	else {
		if (scenario.rightCharaNumOld != -1) {
			chara[scenario.rightCharaNumOld].SetAll(pos[1], scale, 0, charaAlpha[1]);
			chara[scenario.rightCharaNumOld].SetTurn(false, false);
			chara[scenario.rightCharaNumOld].Draw();
		}
	}
}


void NovelSystem::DrawUI(){

	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);

	textBase.Draw();
	frame[0].Draw();
	frame[1].Draw();

	float autoAlpha = -0.25;
	autoAlpha += (autoMode * 0.5f * enable);

	autoButton.SetAll(Vector2(1730, 900), Vector2(1, 1), 0, alphaUI + autoAlpha);
	skipButton.SetAll(Vector2(1730, 1000), Vector2(1, 1), 0, alphaUI);
	autoButton.Draw();
	skipButton.Draw();

	Color strColorAuto(1,1,1, autoButton.GetAlpha());
	Color strColorSkip(1,1,1, skipButton.GetAlpha());

	Vector2 autoStrPos = autoButton.GetPos();
	autoStrPos.X - 10;
	Vector2 skipStrPos = skipButton.GetPos();
	skipStrPos.X - 10;

	akazukin->DrawString("AUTO", autoStrPos, 3.0, strColorAuto, true);
	akazukin->DrawString("SKIP", skipStrPos, 3.0, strColorSkip, true);



	
}
void NovelSystem::DrawScenarioText() {

	auto fonts = Fonts::GetInstance();

	//イニシャライズで設定
	Vector2 strPos[3];
	strPos[0].X = 530;
	for (int i = 0; i < 3; i++) {
		float strPosDistY = 50;
		strPos[i].X = 530;
		strPos[i].Y = 860 + strPosDistY * i;
	}
	float strSize = 2.0f;
	Color strColor(63.0f / 256.0f, 41.0f / 256.0f, 20.0f / 256.0f, alphaUI);

	//キャラネーム描画
	Vector2 charaNamePos = Vector2(585, 775);

	float charaNameScale = 3.0f;
	fonts->GetFont((int)FontList::AKAZUKIN)->DrawString(scenario.str[(int)ScenarioDataName::CHARA_NAME], charaNamePos, charaNameScale, strColor,true);
	//テキスト描画
	for (int i = 0; i < 3; i++) {
		int strNum = (int)ScenarioDataName::SCENARIO_1 + i;
		fonts->GetFont((int)FontList::MEIRIO)->DrawString(scenario.str[strNum], strPos[i], strSize, strColor);
	}


}

void NovelSystem::EndNovel() {
	CameraWork* camera = CameraWork::GetInstance();

	SetEnable(false);
	camera->SetMoveNum(0);
}