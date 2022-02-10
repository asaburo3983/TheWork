#pragma once
#include "Application.h"

#include "Objects.h"

#include "Loading.h"

#include "MaterialLinkerData.h"

#include "GallaryPlayer.h"

#include "EventManager.h"
#include "Stage.h"
#include "Player.h"
#include "SoundManager.h"
using namespace frauEngine;
enum class Button {
	CAMERA,ENABLE,SHADER,POSTEFFECT,MAX
};
enum class PreviewShader {
	PBR,TOON,MAX
};
class GallaryScene : public Scene {
private:
	Loading loading;

	CameraObject camera;


	ImageObject wideButtonImage[4];
	ImageObject cameraStrImage;
	ImageObject enableUIStrImage;

	ImageObject shaderStrImages[2];
	ImageObject postEffectStrImages[(int)AddPostEffect::MAX];

	ImageObject cameraTutorialStrImage;
	ImageObject DoFTutorialStrImage;
	ImageObject skyImage;

	bool enableUI = true;

	bool photoMode = false;

	int shaderNum = 0;
	int postEffectNum = 0;
public:

	void LoadFrontLoad();
	void LoadInUpdate();
	void LoadInDraw();

	void StaticLoad();
	void Load();
	void Update();
	void Draw();
	void DrawNonePostEffect();
	void UnLoad();

};
