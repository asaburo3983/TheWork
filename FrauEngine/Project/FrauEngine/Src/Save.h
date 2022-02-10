#pragma once

#include "EventManager.h"
#include "Item.h"
#include "NormalUI.h"
#include "NovelSystem.h"

using namespace frauEngine;

class Save {
private:
	struct SaveDatas {
		int day = 0;
		int money = 0;

		int itemNum[19];
		int itemGetFlag[19];

		int novelFlag[(int)ScenarioName::MAX];
	};
	SaveDatas saveData;
public:
	static Save* GetInsatnce() {
		static Save s;
		return &s;
	}


	void SaveData();
	void LoadData();

};