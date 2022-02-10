
#include "FlowerShopScene.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void FlowerShopScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
void FlowerShopScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void FlowerShopScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void FlowerShopScene::StaticLoad() {
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();

	HandyShop* handyShop = HandyShop::GetInstance();

	SetObjectList(player->GetModel(), "Player");
	SetObjectList(camera->GetCamera(), "Camera");
	SetObjectList(stage->GetLight(), "Lights");

}

void FlowerShopScene::Load() {
	Stage* stage = Stage::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();
	auto app = Application::GetInstance();
	eventManager->SetScene(this);
	
	if (app->GetSceneOldStr() != "Option") {	
		stage->Load(StageNum::FLOWER_SHOP);
	}
	if (app->GetSceneOldStr() == "Title") {
		stage->MoveStage((int)StageNum::FLOWER_SHOP);
	}
}

void FlowerShopScene::UnLoad() {

}

void FlowerShopScene::Update() {
	CommonUpdate();
}
void FlowerShopScene::Draw() {
	CommonDraw();
}

void FlowerShopScene::DrawNonePostEffect() {
	CommonDrawNonePostEffect();
}
