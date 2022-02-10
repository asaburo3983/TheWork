#include "HandyShop.h"

void HandyShop::Initialize(std::string _itemCSVPath, std::string _textCSVPath) {

	auto rc = Resource::GetInstance();
	npc[0].SetResource(rc->Image("Handy_Normal.png"));
	npc[1].SetResource(rc->Image("Handy_Smile.png"));
	npc[2].SetResource(rc->Image("Handy_Surprise.png"));
	npc[3].SetResource(rc->Image("Handy_Trouble.png"));
	for (int i = 0; i < 4; i++) {
		npc[i].SetAll(Vector2(400, 760), Vector2(0.5, 0.5), 0, alpha);
		npc[i].SetTurn(true, false);
	}
	Shop::Initialize(_itemCSVPath, _textCSVPath);

	itemIcon[0].SetResource(rc->Image("Maso_Seed.png"));

	itemIcon[1].SetResource(rc->Image("Tanepopo_Seed.png"));

	itemIcon[2].SetResource(rc->Image("Beranda_Seed.png"));

	itemIcon[3].SetResource(rc->Image("Panda_Seed.png"));						  
	
	itemIcon[4].SetResource(rc->Image("Rose_Seed.png"));
										
	itemIcon[5].SetResource(rc->Image("Sugger_Seed.png"));

	itemIcon[6].SetResource(rc->Image("Cosmo_Seed.png"));

}

void HandyShop::Update() {
	auto cameraWork = CameraWork::GetInstance();
	Shop::Update();
	if (!enable) {
		if (cameraWork->GetMoveNum() == 2) {
			cameraWork->SetMoveNum(0);
		}
	}

}
void HandyShop::Draw() {

	//—§‚¿ŠG•`‰æ
	npc[charaEcpression].SetAlpha(alpha);
	npc[charaEcpression].Draw();

	Shop::Draw();

	//ƒAƒCƒeƒ€•`‰æ
	if (selectNum != -1) {
		itemIcon[selectNum].SetAll(Vector2(1015, 320), Vector2(1, 1), 0, manualAlpha);
		itemIcon[selectNum].Draw();
	}

}