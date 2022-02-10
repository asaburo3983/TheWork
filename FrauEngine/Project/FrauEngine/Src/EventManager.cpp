#include "EventManager.h"
#include "SoundManager.h"


void EventManager::Initialize() {
	isStartDays = true;
	days = 1;
	fade = false;
	fadeSpeed = 0.05f;
	fadeCount = 0.0f;
}
void EventManager::StartDays() {
	
	if (!isStartDays) {
		return;
	}
	//日にちが変わった時だけに発生するイベント
	Player* player = Player::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();
	auto item = Item::GetInstance();
	auto sound = SoundManager::GetInstance();

	switch (days) {
	case 1:
		player->SetPos(Vector3(4.5, -3.9, 5.2));
		player->SetAngle(Vector3(0, 180, 0));
		novelSystem->SetEnable(true, ScenarioName::PROLOGUE);
		camera->SetMoveNum(1);
		item->AddItem("マジックプランター", 1);//追加
		item->AddItem("マソハーブの種", 3);

		//デバッグ用アイテムの追加
		if (LowApplication::GetInstance()->GetDebugMode()) {
			item->AddItem("マジックプランター", 2);//追加
			item->AddItem("マソハーブの種", 10);//追加
			item->AddItem("タネポポの種", 10);//追加
			item->AddItem("ベランダーの種", 10);//追加
			item->AddItem("パンダジーの種", 10);//追加
			item->AddItem("バララバの種", 10);//追加
			item->AddItem("シュガーステムの種", 10);//追加
			item->AddItem("コスモの種", 10);//追加
		}

		break;
	case 2:
		break;
	case 4:
		if (novelSystem->GetEnd(ScenarioName::HANDYSHOP_JOIN) &&
			novelSystem->GetEnd(ScenarioName::MAGICSHOP_JOIN)) {
			novelSystem->SetEnable(true, ScenarioName::HANDY_EVENT1);
		}
		break;
	case 11:
		if (novelSystem->GetEnd(ScenarioName::HANDY_EVENT1)) {
			novelSystem->SetEnable(true, ScenarioName::MAGIC_EVENT1);
		}
		break;
	case 17:
		if (novelSystem->GetEnd(ScenarioName::MAGIC_EVENT1)) {
			novelSystem->SetEnable(true, ScenarioName::HANDY_EVENT2);
		}
		break;
	case 31:
		//100万ラウなければゲームオーバー
		if (item->GetMoney() < 1000000) {
			novelSystem->SetEnable(true, ScenarioName::BAD_END);
		}
		break;
	}
	//100万ラウたまればその時点でクリア
	if (item->GetMoney() >= 1000000) {
		novelSystem->SetEnable(true, ScenarioName::HAPPY_END);
	}
	isStartDays = false;
}
void EventManager::AddDays() {
	days++;
}
void EventManager::EndDays() {
	isStartDays = true;
}
void EventManager::FieldEvent() {
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();
	auto mouse = MouseInput::GetInstance();
	auto handyShop = HandyShop::GetInstance();
	auto magicShop = MagicShop::GetInstance();
	auto palnterSystem = PlanterSystem::GetInstance();
	auto nextDay = NextDay::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto novelSystem = NovelSystem::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();

	int eventNum = player->GetEventNum();

	//イベント入力処理
	if (mouse->left == 1 && eventNum != -1) {
		
		switch (eventNum) {
		//移動イベント用のフェード
		case (int)EventNum::EXIT:
		case (int)EventNum::JOIN_FLOWERSHOP:
		case (int)EventNum::JOIN_HANDYSHOP:
		case (int)EventNum::JOIN_MAGICSHOP:
			fade = true;
			sound->GetSE(SoundList_SE::DOOR)->Play();
			break;
		case (int)EventNum::PLANTER:
			palnterSystem->SetEnable(true);
			
			break;
		case (int)EventNum::OPEN_SHOP:
			//日付をまたぐ処理、確認入れる
			if (nextDay->GetProcessEnable() == false && novelSystem->GetEnable()==false) {
				nextDay->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
			break;
		case (int)EventNum::BUY_HANDYSHOP:
			if (handyShop->GetEnable() == false) {
				handyShop->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
				camera->SetMoveNum(2);
			}
			break;
		case (int)EventNum::BUY_MAGICSHOP:
			if (magicShop->GetEnable() == false) {
				magicShop->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
				camera->SetMoveNum(3);
			}
			break;
		}
	}
	
	//フェード処理
	if (fade) {
		if (fadeCount <1.0f) {
			fadeCount += fadeSpeed;
		}
		player->IsMove(false);
	}
	else {
		if (fadeCount > 0) {
			fadeCount -= fadeSpeed; 
		}
	}
	//フェードが真っ暗な時の処理
	if (fadeCount >= 1.0f) {
		fade = false;
		player->IsMove(true);
		switch (eventNum) {
			//これはシーン移動の処理にしてしまう
		case (int)EventNum::EXIT:
			
			scene->LoadScene("Map");
			stage->MoveStage((int)StageNum::MAP);

		

			break;
		case (int)EventNum::JOIN_FLOWERSHOP:
			
			scene->LoadScene("FlowerShop");
			stage->MoveStage((int)StageNum::FLOWER_SHOP);
			break;
		case (int)EventNum::JOIN_HANDYSHOP:
			scene->LoadScene("HandyShop");
			stage->MoveStage((int)StageNum::HANDY_SHOP);

			
			break;
		case (int)EventNum::JOIN_MAGICSHOP:
			scene->LoadScene("MagicShop");
			stage->MoveStage((int)StageNum::MAGIC_SHOP);

			
			break;
		}
	}

}
void EventManager::JoinTutorial() {
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();

	//マップ移動をしたときに表示するチュートリアル
	switch (stage->GetStageNum()) {
	case (int)StageNum::FLOWER_SHOP:
		//操作方法のチュートリアル
		tutorialSystem->SetEnable(true, 0);

		//プランターの使い方チュートリアル
		if (tutorialSystem->GetEnd(1)) {
			tutorialSystem->SetEnable(true, 2);
		}
		//カウンターの使い方チュートリアル
		if (tutorialSystem->GetEnd(2)) {
			tutorialSystem->SetEnable(true, 3);
		}
		break;
	case (int)StageNum::MAP:
		break;
	case (int)StageNum::HANDY_SHOP:
		novelSystem->SetEnable(true, ScenarioName::HANDYSHOP_JOIN);
		tutorialSystem->SetEnable(true, 1);
		
		break;
	case (int)StageNum::MAGIC_SHOP:
		novelSystem->SetEnable(true, ScenarioName::MAGICSHOP_JOIN);
		tutorialSystem->SetEnable(true, 1);
		
		break;
	}

}
void EventManager::GameEndEvent() {
	NovelSystem* novelSystem = NovelSystem::GetInstance();

	//とりあえず簡素にscene移動する
	if (novelSystem->GetEnd(ScenarioName::BAD_END) || novelSystem->GetEnd(ScenarioName::HAPPY_END)) {
		scene->LoadScene("Endroll");
	}
}
void EventManager::Update() {
	CameraWork* camera = CameraWork::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	Stage* stage = Stage::GetInstance();
	auto key = KeyInput::GetInstance();

	StartDays();

	JoinTutorial();

	FieldEvent();

	GameEndEvent();
	//マップの時のみカメラは追従させる
	if (stage->GetStageNum() == (int)StageNum::MAP) {
		camera->SetPlayerTarget(true);
	}
	else {
		camera->SetPlayerTarget(false);
	}

	//オプション画面への移行
	//TODO 音楽を止める処理も追加しなければならない
	if (key->key[DIK_ESCAPE] == 1) {
		scene->LoadScene("Option");
	}
}