
#include "HandyShopScene.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void HandyShopScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
void HandyShopScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void HandyShopScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void HandyShopScene::StaticLoad() {
}






void HandyShopScene::Load() {
	Stage* stage = Stage::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();

	eventManager->SetScene(this);
	auto app = Application::GetInstance();
	if (app->GetSceneOldStr() != "Option") {
		stage->Load(StageNum::HANDY_SHOP);
	}
}


void HandyShopScene::UnLoad() {
}

void HandyShopScene::Update() {

	CommonUpdate();

	auto handyShop = HandyShop::GetInstance();
	handyShop->Update();

}
void HandyShopScene::Draw() {
	CommonDraw();
}

void HandyShopScene::DrawNonePostEffect() {

	CommonDrawNonePostEffect();

	auto handyShop = HandyShop::GetInstance();
	handyShop->Draw();
}
