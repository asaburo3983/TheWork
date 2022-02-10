
#include "HandyShopScene.h"

//���[�h�O�̃f�[�^���[�h�@�}���`�X���b�h�Ŏg���f�[�^�Ȃǂ̏�����
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
