#pragma once
#include "Shop.h"

class MagicShop : public Shop {
public:
	static MagicShop* GetInstance() {
		static MagicShop hs;
		return &hs;
	}
	void Initialize(std::string _itemCSVPath, std::string _textCSVPath);
	void Update();
	void Draw();
private:


	ImageObject itemIcon[7];
	ImageObject npc[4];
private:
	void SelectItem();
	void Buy();

};