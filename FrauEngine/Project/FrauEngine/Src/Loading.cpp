#include "Loading.h"

using namespace frauEngine;

void Loading::Initialize() {
	auto resource = Resource::GetInstance();
	backImage.SetResource(resource->LoadIm("Data/Image/Global/Load/LoadBack.png"));
	frontImage.SetResource(resource->LoadIm("Data/Image/Global/Load/LoadFront.png"));
	grassImage.SetResource(resource->LoadIm("Data/Image/Global/Load/LoadGrass.png"));
	for (int i = 0; i < 5; i++) {
		char tmp[256];
		sprintf_s(tmp, "Data/Image/Global/Load/LoadAnim%d.png", i);
		animeImage[i].SetResource(resource->LoadIm(tmp));
	}
}
void Loading::Reset() {
	animeNum = 0;
	animeCount = 0;
	dist = 200;
	posX = -dist;
	
	while (grass.size() > 0) {
		grass.pop_back();
	}
}

void Loading::Updata() {
	const int speed = 5;
	const int posXMax = 1920+dist;

	//キャラの位置を移動
	posX += speed;
	if (posX > posXMax) {
		posX = -dist;
	}

	//アニメーションの変異
	int animChangeF = 7;
	
	if (animeCount % animChangeF == 0) {
		if (animeCount >= animChangeF * 8) {
			animeCount = 0;
		}
		if (animeCount >= animChangeF * 4) {
			animeNum--;
		}
		else {
			animeNum++;
		}
	}
	animeCount++;

	//草を発生させる
	int grassPushF = 45;
	if (animeCount % grassPushF == 0) {
		grass.push_back(posX);
	}

}

void Loading::Draw() {
	Resource* rc = Resource::GetInstance();
	frontAlpha++;

	if (frontAlpha > 100) {
		frontAlpha = 0;
	}
	//背景
	backImage.SetAll(Vector2(960, 540));
	backImage.Draw();

	frontImage.SetAll(Vector2(960, 540), Vector2(1, 1), 0, (float)frontAlpha / 100.0f);
	frontImage.Draw();


	//草
	for (int i = 0; i < grass.size(); i++) {
		grassImage.SetPos(Vector2(grass[i], 670));
		grassImage.Draw();
	}

	//アニメ
	animeImage[animeNum].SetAll(Vector2(posX, 570), Vector2(0.6, 0.6), 0, 1);
	animeImage[animeNum].SetTurn(true,false);
	animeImage[animeNum].Draw();

}