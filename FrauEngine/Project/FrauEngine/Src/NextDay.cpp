#include "NextDay.h"
#include "SoundManager.h"

void NextDay::Initialize() {

	auto rc = Resource::GetInstance();

	base.SetResource(rc->Image("MoneyBase.png"));
	base.SetAll(Vector2(960, 540), Vector2(2, 5));
	
	morningBase.SetResource(rc->Image("MoneyBase.png"));
	morningBase.SetAll(Vector2(960, 540), Vector2(2, 8));

	Vector2 dist(160, 100);
	Vector2 buttonScale(0.7, 1);

	yes.SetResource(rc->Image("MoneyBase.png"));
	yes.SetAll(Vector2(960-dist.X, 540+dist.Y), buttonScale);

	no.SetResource(rc->Image("MoneyBase.png"));
	no.SetAll(Vector2(960 + dist.X, 540 + dist.Y), buttonScale);

	for (int i = 0; i < 3; i++) {
		seed[0][i].SetResource(rc->Image("Tanepopo_Seed.png"));
		seed[1][i].SetResource(rc->Image("Masomaso_Seed.png"));
		seed[2][i].SetResource(rc->Image("Ranpopo_Seed.png"));
		seed[3][i].SetResource(rc->Image("Barabara_Seed.png"));
		seed[4][i].SetResource(rc->Image("SuggerPan_Seed.png"));
		seed[5][i].SetResource(rc->Image("Frau_Seed.png"));
	}
}
void NextDay::Update() {
	auto mouse = MouseInput::GetInstance();
	auto eventManager = EventManager::GetInstance();
	auto planterSystem = PlanterSystem::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto player = Player::GetInstance();

	//確認用の処理
	if (enable&& nightAlpha <= 0.1) {
		if (fade < 1) {
			fade += fadeSpeed;
		}
		//ボタンで確認事項を決定
		if (mouse->left == 1) {
			if (yes.Hit(mouse->x, mouse->y)) {
				night = true;
				SetEnable(false);
				sound->GetSE(SoundList_SE::NEXTDAY)->Play();
			}
			if (no.Hit(mouse->x, mouse->y)) {
				SetEnable(false);
				processEnable = false;
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
		}
		//右クリックでもキャンセルが可能
		if (mouse->right == 1) {
			SetEnable(false);
			processEnable = false;
			sound->GetSE(SoundList_SE::ENTER)->Play();
		}
	}
	else {
		if (fade > 0) {
			fade -= fadeSpeed;
		}
	}
	//次の日に行くフェード		黒いフェード
	if (night) {
		if (nightAlpha < nightAlphaMax) {
			nightAlpha += fadeSpeed;
		}
		else {
			night = false;
			morning = true;
			
			eventManager->AddDays();

			//金額の増加処理を行う
			//アイテムの消費を行う
			//交配した種の生成を行う
			planterSystem->SeedGrow();

			//データをセーブする
			Save::GetInsatnce()->SaveData();

			//プレイヤーの位置を設定する
			//プレイヤーの移動を制限する
			//player->SetPos(Vector3(1.0, -3.9, 0));
			player->SetPos(Vector3(4.5, -3.9, 5.2));
			player->SetAngle(Vector3(0, 180, 0));
			
		}
	}
	else {
		if (nightAlpha > 0) {
			nightAlpha -= fadeSpeed;
		}
	}
	//次の日の初めの処理
	if (morning) {
		if (mouse->left == 1 && nightAlpha <= 0.1) {
			morning = false;
			eventManager->EndDays();
		
			//プランターの中身を殻にする
			planterSystem->Reset();
			processEnable = false;//すべての処理が終了

			player->IsMove(true);

		}
		if (morningAlpha < 1) {
			morningAlpha += fadeSpeed;
		}
	}
	else {
		if (morningAlpha > 0) {
			morningAlpha -= fadeSpeed;
		}
	}

	base.SetAlpha(fade);
	yes.SetAlpha(fade);
	no.SetAlpha(fade);

}

void NextDay::Draw() {
	DrawCheck();
	DrawMorning();
}
void NextDay::DrawCheck() {
	if (fade <= 0) {
		return;
	}

	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	base.Draw();
	yes.Draw();
	no.Draw();
	Vector2 manualStrPos = base.GetPosLerp(Vector2(0.4, 0.2));
	akazukin->DrawString(str, manualStrPos, 2, Color(63, 41, 20, fade), true);

	akazukin->DrawString("はい", Vector2(yes.GetPos().X-15, yes.GetPos().Y-10), 2.5, Color(63, 41, 20, fade), true);
	akazukin->DrawString("いいえ", Vector2(no.GetPos().X - 15, no.GetPos().Y-10), 2.5, Color(63, 41, 20, fade), true);
	
}
void NextDay::DrawMorning() {
	if (morningAlpha <= 0) {
		return;
	}
	auto item = Item::GetInstance();
	auto planterSystem = PlanterSystem::GetInstance();
	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	Color strColor = Color(63, 41, 20, morningAlpha);
	//ベースの描画（イメージ
	morningBase.SetAlpha(morningAlpha);
	morningBase.Draw();

	//本日の成果(文字列
	Vector2 manualStrPos = morningBase.GetPosLerp(Vector2(0.5, 0.2));
	akazukin->DrawString("本日の成果！", manualStrPos, 3, Color(63, 41, 20, morningAlpha), true);
	//基本収入
	Vector2 standardMoneyStrPos = morningBase.GetPosLerp(Vector2(0.4, 0.25));
	meirio->DrawString("基本収入:100ラウ",standardMoneyStrPos, 1.7, strColor,true);
	//
	Vector2 matiTitlePos = morningBase.GetPosLerp(Vector2(0.4, 0.6));
	meirio->DrawString("花からとれた種", matiTitlePos, 1.7, strColor, true);
	//育成可否（文字列
	for (int i = 0; i < item->GetItemNum("マジックプランター"); i++) {
		int setItem = planterSystem->GetSetItem(i);
		if (setItem == -1) {
			continue;
		}
		bool seedGrow = planterSystem->GetGrouSeedEnable(i);
		string matingSeedStr = "";
		if (planterSystem->GetMatingSeed(i) != -1) {
			matingSeedStr = item->GetItemName(planterSystem->GetMatingSeed(i));
		}


		Vector2 seedNamePos = morningBase.GetPosLerp(Vector2(0.1,  0.4+(i*0.05)));
		Vector2 seedGrowPos = morningBase.GetPosLerp(Vector2(0.4,  0.4+(i*0.05)));
		Vector2 seedMoneyPos = morningBase.GetPosLerp(Vector2(0.65, 0.4+(i*0.05)));

		float strScale = 1.5f;
		//同じ列///////////////////////
		//育成した種
		string setSeedStr = item->GetItemName(setItem);
		meirio->DrawString(setSeedStr, seedNamePos, strScale, strColor);
		//成功or失敗
		string growStr = "育成失敗";
		if (seedGrow) {
			growStr = "育成成功！";
		}
		meirio->DrawString(growStr,seedGrowPos, strScale, strColor);
		//種ごとの収入
		char moneyStr[50] = "0ラウ";
		if (seedGrow) {
			sprintf_s(moneyStr,"%dラウ", item->GetSell(setItem));
		}
		meirio->DrawString(moneyStr, seedMoneyPos, strScale, strColor);
		/////////////////////////////////
		
		//交配でゲットした種
		//同じ列///////////////
		Vector2 matiSeedPos = morningBase.GetPosLerp(Vector2(0.25 + i * 0.25, 0.80f));

		for (int h = 0; h < 6; h++) {
			seed[h][i].SetAll(matiSeedPos, Vector2(0.5, 0.5), 0, morningAlpha);
		}
		int getSeed = planterSystem->GetMatingSeed(i);
		if (getSeed == 1) {
			seed[0][i].Draw();
		}
		else if (getSeed != -1) {
			seed[getSeed - 6][i].Draw();
		}

		Vector2 matiSeedPosStr = morningBase.GetPosLerp(Vector2(0.25 + i * 0.25, 0.7));
		meirio->DrawString(matingSeedStr, matiSeedPosStr, strScale, strColor, true);
		///////////////////////
	}
	
	
	

}