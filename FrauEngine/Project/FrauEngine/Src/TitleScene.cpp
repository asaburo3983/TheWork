
#include "TitleScene.h"

//���[�h�O�̃f�[�^���[�h�@�}���`�X���b�h�Ŏg���f�[�^�Ȃǂ̏�����
void TitleScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
//���[�h���̏���
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


	//�������s�����N�������̂ŃV�[�����̃������͂SGB�܂łɗ}���邽�߁A���񃍁[�h�A�A�����[�h����
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

	//�Q�[���p�f�[�^������������
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

	//�I���������摜�̈ʒu�ݒ�
	float hitSelect = false;
	if (fadeOn == false) {
		//�}�E�X�ɂ��I��
		for (int i = 0; i < (int)TitleStr::MAX; i++) {

			Vector2 pos = Vector2(std::lerp((float)strImageMoveStartX, (float)strImageMoveEndX, strImageLerp[i]), strImageStartY + strImageDistY * i);
			titleStr[i].SetAll(pos);
			
			if (titleStr[i].Hit(mouse->x, mouse->y)) {
				cursorNum = i;
				hitSelect = true;
			}
		}
		//�I�����������摜�̈ړ�
		for (int i = 0; i < (int)TitleStr::MAX; i++) {
			if (i == cursorNum && strImageLerp[i] < 1.0f) {
				strImageLerp[i] += strImageLerpSpeed;
			}
			else if (i != cursorNum && strImageLerp[i] > 0.0f) {
				strImageLerp[i] -= strImageLerpSpeed;
			}
		}
	}
	//�J�[�\���̈ʒu�ݒ�
	Vector2 cursorPos = Vector2(cursorX, strImageStartY + strImageDistY * cursorNum);
	titleCursor.SetAll(cursorPos);

	if (mouse->left && hitSelect) {
		sound->GetSE(SoundList_SE::ENTER)->Play();
		fadeOn = true;
		sceneMove = true;
	}
	//scene�ړ�
	if (sceneMove) {
		sceneMove = false;
		sound->GetBGM(SoundList_BGM::TITLE)->Stop();
		switch (cursorNum) {
		case (int)TitleStr::START:
			//�Z�[�u�f�[�^������ꍇ���[�h����
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

