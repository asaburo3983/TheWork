#pragma once


#include "Application.h"
#include "ImageObject.h"

using namespace frauEngine;

class Loading {
private:
	int animeNum = 0;
	int animeCount = 0;
	int posX = 0;
	int dist = 200;
	std::vector<int> grass;
	
	int frontAlpha = 0;

	ImageObject backImage;
	ImageObject frontImage;
	ImageObject grassImage;
	ImageObject animeImage[5];

public:
	static Loading* GetInstance() {
		static Loading loading;
		return &loading;
	}

	void Initialize();
	void Reset();
	void Updata();
	void Draw();
};