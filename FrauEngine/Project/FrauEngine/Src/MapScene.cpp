
#include "MapScene.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void MapScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
void MapScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void MapScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void MapScene::StaticLoad() {
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();

	HandyShop* handyShop = HandyShop::GetInstance();
}






void MapScene::Load() {
	Stage* stage = Stage::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();
	eventManager->SetScene(this);
	auto app = Application::GetInstance();
	if (app->GetSceneOldStr() != "Option") {
		stage->Load(StageNum::MAP);
	}

}


void MapScene::UnLoad() {
}

void MapScene::Update() {
	CommonUpdate();
}
void MapScene::Draw() {
	CommonDraw();
}

void MapScene::DrawNonePostEffect() {
	CommonDrawNonePostEffect();
}
