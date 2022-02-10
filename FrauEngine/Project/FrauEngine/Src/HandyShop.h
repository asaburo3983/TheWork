#pragma once
#include "Shop.h"

class HandyShop : public Shop {
public:
	static HandyShop* GetInstance() {
		static HandyShop hs;
		return &hs;
	}
	void Initialize(std::string _itemCSVPath, std::string _textCSVPath);
	void Update();
	void Draw();
private:


	ImageObject itemIcon[7];
	ImageObject npc[4];

};