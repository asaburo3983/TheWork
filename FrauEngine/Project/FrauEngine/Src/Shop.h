#pragma once
#include "Application.h"
#include "Objects.h"
#include "Item.h"
#include "Player.h"
#include "CameraWork.h"

using namespace frauEngine;

class Shop {
protected:
	//このクラスの有効
	bool enable = false;
	//場面
	enum class State {
		NORMAL,
		SELECT,
		BUY,
		NOT_MONEY,
		BYE
	};
	enum class Ecpression {
		NORMAL,
		SMILE,
		SURPRISE,
		TROUBLE
	};
	State state = State::NORMAL;

	//アイテムリスト
	int itemValueNum = 0;
	struct ItemParam {
		int price = 0;
		std::string str1;
		std::string str2;
	};
	std::vector<std::string> itemName;
	std::vector<ItemParam> itemParam;
	//場面におけるリアクション
	struct ReactionParam {
		std::string name;
		std::string str1;
	};
	std::vector<ReactionParam > reactionParam;

	//リソース
	ImageObject shopBase;
	ImageObject shopBaseLine[7];
	ImageObject manualBase;
	ImageObject selectFrame;

	ImageObject textBase;
	ImageObject rightArrow;
	ImageObject leftArrow;
	ImageObject buyButton;

	CSV csvItem;
	CSV csvText;

	//ON_OFFで使う
	const float alphaSpeed = 0.05;
	float alpha = 0.0f;
	//アイテムの選択
	int selectNum = 0;
	//購入個数と合計金額
	int buyNum = 0;
	int allPrice = 0;

	//テキストの固定値
	const Vector2 strPos = Vector2(240, 880);
	const float strFloat = 2.0f;
	Color strColor = Color(63.0f / 256.0f, 41.0f / 256.0f, 20.0f / 256.0f, 1);

	//立ち絵の種類
	int charaEcpression = 0;

	float manualAlpha = 0.0f;
public:
	void Initialize(std::string _itemCSVPath,std::string _textCSVPath);
	void SetEnable(bool _enable);
	void Update();
	void Draw();

	int GetBuyNum() { return buyNum; }
	int GetAllPrice() { return allPrice; }
	bool GetEnable() { return enable; }
	float GetAlpha() { return alpha; }
protected:
	void Cancel();
	void SelectItem();
	void SelectBuyNum();
	void Buy();
};
