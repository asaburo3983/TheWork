#include "EndrollScene.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void EndrollScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
void EndrollScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void EndrollScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void EndrollScene::StaticLoad() {
	auto rc = Resource::GetInstance();
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	EventManager* eventManager = EventManager::GetInstance();

	HandyShop* handyShop = HandyShop::GetInstance();

	black.SetResource(rc->Image("Black.png"));
	csv.LoadCSV("Data/EndrollCSV/Endroll.csv");

	black.SetAll(Vector2(960, 540));
}






void EndrollScene::Load() {

}


void EndrollScene::UnLoad() {
}

void EndrollScene::Update() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();
	sound->GetBGM(SoundList_BGM::ENDROLL)->Play();

	if (scrollCountMax > scrollCount) {
		scrollCount += scrollCountSpeed;
	}
	else {
		if (mouse->left == 1 || mouse->right == 1) {
			LoadScene("Title");
			sound->GetBGM(SoundList_BGM::ENDROLL)->Stop();
			//セーブデータの削除
			remove("Data/Save/Save.dat");
		}
	}
}
void EndrollScene::Draw() {
	auto app = Application::GetInstance();
	app->SetDepthOfField(true);
	app->SetAddEffect((int)AddPostEffect::NONE);
}

void EndrollScene::DrawNonePostEffect() {

	//auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	black.Draw();

	for (int i = 1; i < csv.GetRowMax(); i++) {
		string str1 = csv.GetString(i, 0);
		string str2 = csv.GetString(i, 1);
		Vector2 pos = startPos;
		pos.Y += stringDist.Y * i - scrollCount;
		meirio->DrawString(str1, pos, 2, Color(1, 1, 1, 1));
		pos.X += stringDist.X;
		meirio->DrawString(str2, pos, 2, Color(1, 1, 1, 1));
	}
}
