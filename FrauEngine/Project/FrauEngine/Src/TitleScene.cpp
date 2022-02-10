
#include "TitleScene.h"

//ロード前のデータロード　マルチスレッドで使うデータなどの初期化
void TitleScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
//ロード中の処理
void TitleScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void TitleScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void TitleScene::StaticLoad() {

	CommonDataLoad();

}


void TitleScene::Load() {


	//メモリ不足が起こったのでシーン内のメモリは４GBまでに抑えるため、毎回ロード、アンロードする
	auto TitleFront = resource->LoadIm("Data/Image/Title/TitleFront.png");
	auto TitleBack = resource->LoadIm("Data/Image/Title/TitleBack.png");
	auto TitleCursor = resource->LoadIm("Data/Image/Title/TitleCursor.png");
	auto StartStr = resource->LoadIm("Data/Image/Title/StartStr.png");
	auto GallaryStr = resource->LoadIm("Data/Image/Title/GallaryStr.png");
	auto OptionStr = resource->LoadIm("Data/Image/Title/OptionStr.png");
	auto ExitStr = resource->LoadIm("Data/Image/Title/ExitStr.png");
	auto cloud = resource->LoadIm("Data/Image/Title/TitleCloud.png");

	titleCloud.SetResource(cloud);
	titleCloud.SetAll(Vector2(960, 540));

	titleFront.SetResource(TitleFront);
	titleFront.SetAll(Vector2(960, 540));

	titleBack.SetResource(TitleBack);
	titleBack.SetAll(Vector2(960, 540));

	titleCursor.SetResource(TitleCursor);
	titleStr[0].SetResource(StartStr);
	titleStr[1].SetResource(GallaryStr);
	titleStr[2].SetResource(OptionStr);
	titleStr[3].SetResource(ExitStr);

	fadeOn = false;
	sceneMove = false;

	//ゲーム用データを初期化する
	CommonReset();
}
void TitleScene::UnLoad() {
	resource->UnLoadIm("TitleFront.png");
	resource->UnLoadIm("TitleBack.png");
	resource->UnLoadIm("TitleCursor.png");
	resource->UnLoadIm("StartStr.png");
	resource->UnLoadIm("GallaryStr.png");
	resource->UnLoadIm("OptionStr.png");
	resource->UnLoadIm("ExitStr.png");
	resource->UnLoadIm("TitleCloud.png");
}

void TitleScene::Update() {
	auto sound = SoundManager::GetInstance();
	sound->GetBGM(SoundList_BGM::TITLE)->Play();

	//選択肢文字画像の位置設定
	float hitSelect = false;
	if (fadeOn == false) {
		//マウスによる選択
		for (int i = 0; i < (int)TitleStr::MAX; i++) {

			Vector2 pos = Vector2(std::lerp((float)strImageMoveStartX, (float)strImageMoveEndX, strImageLerp[i]), strImageStartY + strImageDistY * i);
			titleStr[i].SetAll(pos);
			
			if (titleStr[i].Hit(mouse->x, mouse->y)) {
				cursorNum = i;
				hitSelect = true;
			}
		}
		//選択した文字画像の移動
		for (int i = 0; i < (int)TitleStr::MAX; i++) {
			if (i == cursorNum && strImageLerp[i] < 1.0f) {
				strImageLerp[i] += strImageLerpSpeed;
			}
			else if (i != cursorNum && strImageLerp[i] > 0.0f) {
				strImageLerp[i] -= strImageLerpSpeed;
			}
		}
	}
	//カーソルの位置設定
	Vector2 cursorPos = Vector2(cursorX, strImageStartY + strImageDistY * cursorNum);
	titleCursor.SetAll(cursorPos);

	if (mouse->left && hitSelect) {
		sound->GetSE(SoundList_SE::ENTER)->Play();
		fadeOn = true;
		sceneMove = true;
	}
	//scene移動
	if (sceneMove) {
		sceneMove = false;
		sound->GetBGM(SoundList_BGM::TITLE)->Stop();
		switch (cursorNum) {
		case (int)TitleStr::START:
			//セーブデータがある場合ロードする
			Save::GetInsatnce()->LoadData();
			LoadScene("FlowerShop");
			break;
		case (int)TitleStr::GALLARY:
			LoadScene("Gallary");
			break;
		case (int)TitleStr::OPTION:
			LoadScene("Option");
			break;
		case (int)TitleStr::EXIT:
			gameEnd = true;
			break;
		}
	}

}
 
void TitleScene::Draw() {
	auto app = Application::GetInstance();
	

	

	titleBack.Draw();
	
	titleFront.Draw();

	titleCursor.Draw();

	
	for (int i = 0; i < (int)TitleStr::MAX; i++) {
		titleStr[i].Draw();
	}

	titleCloud.Draw();

	app->SetColorMulti();

	
}
void TitleScene::DrawNonePostEffect() {
}

