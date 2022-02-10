#pragma once
#include "Application.h"
#include "Objects.h"
#include "Item.h"
#include "EventManager.h"

using namespace frauEngine;

class NormalUI {
public:
	static NormalUI* GetInstance() {
		static NormalUI n;
		return &n;
	}
	void Initialize();
	void Update();
	void Draw();
	void Reset() {
		//�C���x���g���`��ŕK�v
		inventory = false;
		inventoryAlpha = 0.0f;
		selectItemNum = 0;
		page = 1;
	}
private:

	ImageObject dayBase;
	
	ImageObject itemBase;
	ImageObject itemLine[7];
	ImageObject manualBase;
	ImageObject selectFrame;
	ImageObject rightArrow;
	ImageObject leftArrow;

	ImageObject moneyBase;

	ImageObject items[19];


	//�C���x���g���`��ŕK�v
	bool inventory = false;
	float inventoryAlpha = 0.0f;
	const float inventoryAlphaSpeed = 0.05f;
	int selectItemNum = 0;
	int page = 1;
};