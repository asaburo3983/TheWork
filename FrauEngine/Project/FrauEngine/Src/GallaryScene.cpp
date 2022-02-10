
#include "GallaryScene.h"
//#include "CustomMaterial.h"


//���[�h�O�̃f�[�^���[�h�@�}���`�X���b�h�Ŏg���f�[�^�Ȃǂ̏�����
void GallaryScene::LoadFrontLoad() {
	Loading::GetInstance()->Reset();
}
void GallaryScene::LoadInUpdate() {
	Loading::GetInstance()->Updata();
}
void GallaryScene::LoadInDraw() {
	Loading::GetInstance()->Draw();
}

void GallaryScene::StaticLoad() {

	//UI
	auto wideButton = resource->LoadIm("Data/Image/Gallary/WideButon.png");
	for (int i = 0; i < (int)Button::MAX; i++) {
		wideButtonImage[i].SetResource(wideButton);
	}

	cameraStrImage.SetResource(resource->LoadIm("Data/Image/Gallary/Str/CameraStr.png"));
	enableUIStrImage.SetResource(resource->LoadIm("Data/Image/Gallary/Str/EnableUIStr.png"));

	shaderStrImages[0].SetResource(resource->LoadIm("Data/Image/Gallary/Str/PBRShaderStr.png"));
	shaderStrImages[1].SetResource(resource->LoadIm("Data/Image/Gallary/Str/ToonShaderStr.png"));

	postEffectStrImages[0].SetResource(resource->LoadIm("Data/Image/Gallary/Str/NoneEffectStr.png"));
	postEffectStrImages[1].SetResource(resource->LoadIm("Data/Image/Gallary/Str/DepthOfFieldStr.png"));
	postEffectStrImages[2].SetResource(resource->LoadIm("Data/Image/Gallary/Str/BlurStr.png"));
	postEffectStrImages[3].SetResource(resource->LoadIm("Data/Image/Gallary/Str/SharpStr.png"));
	postEffectStrImages[4].SetResource(resource->LoadIm("Data/Image/Gallary/Str/RetoroStr.png"));
	postEffectStrImages[5].SetResource(resource->LoadIm("Data/Image/Gallary/Str/NegativeStr.png"));
	postEffectStrImages[6].SetResource(resource->LoadIm("Data/Image/Gallary/Str/OutlineStr.png"));
	postEffectStrImages[7].SetResource(resource->LoadIm("Data/Image/Gallary/Str/OutlineDoFStr.png"));

	cameraTutorialStrImage.SetResource(resource->LoadIm("Data/Image/Gallary/Str/CameraTutorialStr.png"));
	DoFTutorialStrImage.SetResource(resource->LoadIm("Data/Image/Gallary/Str/DoFTutorialStr.png"));

	//�w�i�摜
	skyImage.SetResource(resource->Image("Sky.png"));

	auto stage = Stage::GetInstance();
	stage->Load(StageNum::FLOWER_SHOP);
	stage->MoveStage((int)StageNum::FLOWER_SHOP);


}
void GallaryScene::Load() {
	auto player = Player::GetInstance();
	auto stage = Stage::GetInstance();
	auto camera = CameraWork::GetInstance();
	auto eventManager = EventManager::GetInstance();

	eventManager->SetScene(this);

	stage->Load(StageNum::FLOWER_SHOP);
	//stage->Load(StageNum::HANDY_SHOP);

	player->GetModel()->SetAnotherResourceEnable(1, false);

	//UI�̈ʒu�ݒ�
	Vector2 cameraButtonPos =  Vector2(335, 845);
	Vector2 enableButtonPos =  Vector2(125, 845);

	Vector2 smallButtonUIScale = Vector2(0.45, 1);
	Vector2 smallButtonStrUIScale = Vector2(0.75, 0.75);

	wideButtonImage[(int)Button::CAMERA].SetAll(cameraButtonPos, smallButtonUIScale);
	cameraStrImage.SetAll(cameraButtonPos, smallButtonStrUIScale);

	wideButtonImage[(int)Button::ENABLE].SetAll(enableButtonPos, smallButtonUIScale);
	enableUIStrImage.SetAll(enableButtonPos, smallButtonStrUIScale);

	Vector2 shaderButtonPos = Vector2(230, 930);
	wideButtonImage[(int)Button::SHADER].SetAll(shaderButtonPos);
	for (int i = 0; i < (int)PreviewShader::MAX; i++) {
		shaderStrImages[i].SetAll(shaderButtonPos);
	}

	Vector2 postEffectButtonPos = Vector2(230, 1030);
	wideButtonImage[(int)Button::POSTEFFECT].SetAll(postEffectButtonPos);
	for (int i = 0; i < (int)AddPostEffect::MAX; i++) {
		postEffectStrImages[i].SetAll(postEffectButtonPos);
	}

	cameraTutorialStrImage.SetAll(Vector2(270, 770));
	DoFTutorialStrImage.SetAll(Vector2(270, 720));
	skyImage.SetAll(Vector2(960, 540));

}


void GallaryScene::UnLoad() {
	auto player = Player::GetInstance();
	auto camera = CameraWork::GetInstance();
	enableUI = true;
	photoMode = false;
	shaderNum = 0;
	postEffectNum = 0;
	camera->SetPhotoMode(photoMode);
	player->GetModel()->SetAnotherResourceEnable(1, true);

}


void GallaryScene::Update() {
	
	auto app = Application::GetInstance();

	auto player = Player::GetInstance();
	auto stage = Stage::GetInstance();
	auto camera = CameraWork::GetInstance();
	auto eventManager = EventManager::GetInstance();


	auto sound = SoundManager::GetInstance();
	sound->GetBGM(SoundList_BGM::FLOWER_SHOP)->Play();


	if (mouse->right == 1) {
		LoadScene("Title");
		sound->GetBGM(SoundList_BGM::FLOWER_SHOP)->Stop();
	}

	//UI�̕\��
	if (wideButtonImage[(int)Button::ENABLE].Hit(mouse->x, mouse->y) && mouse->left == 1) {
		enableUI = !enableUI;
		//UI�̓���
		if (enableUI == false) {
			float enableButtonMinAlpha = 0.5f;
			wideButtonImage[(int)Button::ENABLE].SetAlpha(enableButtonMinAlpha);
			enableUIStrImage.SetAlpha(enableButtonMinAlpha);
		}
		//UI�̔񓧉�
		else {
			wideButtonImage[(int)Button::ENABLE].SetAlpha(1);
			enableUIStrImage.SetAlpha(1);
		}
	}

	//�t�H�g���[�h�ؑ�
	if (wideButtonImage[(int)Button::CAMERA].Hit(mouse->x, mouse->y) && mouse->left == 1) {
		photoMode = !photoMode;
		camera->SetPhotoMode(photoMode);
	}

	player->Update();
	stage->Update();
	camera->Updata();

	Application::GetInstance()->SetDepthOfField(true, mouse->x, mouse->y);

	//�V�F�[�_�[�؂�ւ�
	if (wideButtonImage[(int)Button::SHADER].Hit(mouse->x, mouse->y) && mouse->left == 1) {
		shaderNum = !shaderNum;
		if (shaderNum == 0) {
			player->GetModel()->SetAnotherResourceEnable(1, false);
		}		
		if (shaderNum == 1) {
			player->GetModel()->SetAnotherResourceEnable(1, true);
		}


	}
	//�|�X�g�G�t�F�N�g�؂�ւ�
	if (wideButtonImage[(int)Button::POSTEFFECT].Hit(mouse->x, mouse->y) && mouse->left == 1) {
		postEffectNum = (postEffectNum + 1) % (int)AddPostEffect::MAX;
		app->SetBlurPower(1);
	}
	Application::GetInstance()->SetAddEffect(postEffectNum);
}

void GallaryScene::Draw() {

	auto player = Player::GetInstance();
	auto stage = Stage::GetInstance();
	auto camera = CameraWork::GetInstance();
	auto eventManager = EventManager::GetInstance();

	auto lowApp = LowApplication::GetInstance();
	auto lights = Lights::GetInstance();
	auto app = Application::GetInstance();

	//�e�̕`��
	lowApp->DrawOnDepth(Lights::GetInstance()->depthHeap, Lights::GetInstance()->shadowTexSize);

	stage->DrawShadow();
	player->DrawShadow();

	//�ʏ�̕`��
	lowApp->DrawOnRenderTarget(Application::GetInstance()->GetPostEffectRenderTarget());

	//��̕`��
	skyImage.Draw();
	
	stage->Draw();
	player->Draw();
	

}

void GallaryScene::DrawNonePostEffect() {

	wideButtonImage[(int)Button::ENABLE].Draw();
	enableUIStrImage.Draw();

	if (enableUI) {
		for (int i = 0; i < (int)Button::MAX; i++) {
			if (i == (int)Button::ENABLE) {
				continue;
			}
			wideButtonImage[i].Draw();
		}

		cameraStrImage.Draw();

		shaderStrImages[shaderNum].Draw();
		postEffectStrImages[postEffectNum].Draw();

		if (photoMode) {
			cameraTutorialStrImage.Draw();
		}
		if (postEffectNum == (int)AddPostEffect::DEPTHOFFIELD ||
			postEffectNum == (int)AddPostEffect::OUTLINE_DEPTHOFFIELD) {
			DoFTutorialStrImage.Draw();
		}
	}
}
