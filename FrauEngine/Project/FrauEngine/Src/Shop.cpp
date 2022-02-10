#include "Shop.h"
#include "SoundManager.h"

void Shop::SetEnable(bool _enable) {
	auto player = Player::GetInstance();
	enable = _enable;
	if (enable) {
		state = State::NORMAL;
		player->IsMove(false);
	}
	else {
		player->IsMove(true);
	}
}
void Shop::Initialize(std::string _itemCSVPath, std::string _textCSVPath) {
	Resource* rc = Resource::GetInstance();

	csvItem.LoadCSV(_itemCSVPath);
	csvText.LoadCSV(_textCSVPath);

	//商品と説明文、値段
	itemValueNum = csvItem.GetRowMax() - 1;
	itemName.resize(itemValueNum);
	itemParam.resize(itemValueNum);
	for (int i = 0; i < itemValueNum; i++) {		
		itemName[i] = csvItem.GetString(i+1, 0);
		itemParam[i].str1 = csvItem.GetString(i+1, 1);
		itemParam[i].str2 = csvItem.GetString(i+1, 2);
		itemParam[i].price= atoi(csvItem.GetString(i+1, 3).c_str());
	}
	//リアクションのテキスト
	int sizeTmp = csvText.GetRowMax()-1;
	reactionParam.resize(sizeTmp);
	for (int i = 0; i < sizeTmp; i++) {
		reactionParam[i].name = csvText.GetString(i+1, 0);
		reactionParam[i].str1 = csvText.GetString(i+1, 1);

	}
	
	//UI系統　画像の設定と固定のものは配置
	shopBase.SetResource(rc->Image("ShopBase.png"));
	shopBase.SetAll(Vector2(1575, 620));

	for (int i = 0; i < 7; i++) {
		shopBaseLine[i].SetResource(rc->Image("ShopBaseLine.png"));
		int y = 340 + shopBaseLine[i].GetHeight() * i;
		shopBaseLine[i].SetAll(Vector2(1575, y));
	}
	manualBase.SetResource(rc->Image("ManualBase.png"));
	manualBase.SetAll(Vector2(1010, 410));

	textBase.SetResource(rc->Image("Novel_TextBase.png"));
	textBase.SetAll(Vector2(650, 910), Vector2(0.85, 0.85));

	rightArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetTurn(true, false);
	rightArrow.SetAll(Vector2(550, 960), Vector2(0.5, 0.5),0,0);
	leftArrow.SetAll(Vector2(300, 960), Vector2(0.5, 0.5),0,0);

	buyButton.SetResource(rc->Image("BuyButton.png"));
	buyButton.SetAll(Vector2(1020, 970));

	selectFrame.SetResource(rc->Image("SelectFrame.png"));
	
}
void Shop::Cancel() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();

	if (mouse->right == 1) {
		switch (state) {
		case State::NORMAL:
			state = State::BYE;
			mouse->right++;
			sound->GetSE(SoundList_SE::ENTER)->Play();
			break;
		case State::SELECT:
			state = State::NORMAL;
			sound->GetSE(SoundList_SE::ENTER)->Play();
			break;
		}
	}
	if (mouse->left == 1 || mouse->right == 1) {
		switch (state) {
		case State::BUY:
		case State::NOT_MONEY:
			state = State::NORMAL;
			sound->GetSE(SoundList_SE::ENTER)->Play();
			break;
		case State::BYE:
			SetEnable(false);
			sound->GetSE(SoundList_SE::ENTER)->Play();
			break;
		}

		
	}
}
void Shop::SelectItem() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();

	if (mouse->left == 1|| mouse->left % 20==1) {
		for (int i = 0; i < itemValueNum; i++) {
			if (shopBaseLine[i].Hit(mouse->x, mouse->y)) {
				selectNum = i;
				buyNum = 1;
				state = State::SELECT;
				sound->GetSE(SoundList_SE::ENTER)->Play();
				break;
			}
		}
	}
}
void Shop::SelectBuyNum() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();
	if (mouse->left == 1) {
		if (rightArrow.Hit(mouse->x, mouse->y)) {
			buyNum++;
			sound->GetSE(SoundList_SE::ENTER)->Play();
		}
		else if (leftArrow.Hit(mouse->x, mouse->y)) {
			buyNum--;
			sound->GetSE(SoundList_SE::ENTER)->Play();
		}
		if (buyNum < 1) {
			buyNum = 9;
		}
		if (buyNum > 9) {
			buyNum = 1;
		}
	}
	allPrice = buyNum * itemParam[selectNum].price;
}
void Shop::Buy() {
	auto mouse = MouseInput::GetInstance();
	auto item = Item::GetInstance();
	auto sound = SoundManager::GetInstance();
	if (mouse->left == 1) {
		if (buyButton.Hit(mouse->x, mouse->y)) {
			if (item->GetMoney() < allPrice) {
				//お金が足りない
				state = State::NOT_MONEY;
			}
			else {
				//購入処理
				item->AddItem(itemName[selectNum], buyNum);
				item->AddMoney(-allPrice);
				state = State::BUY;
				sound->GetSE(SoundList_SE::BUY)->Play();
			}

		}
	}
}
void Shop::Update() {

	if (enable) {
		Cancel();
		//プレイヤーの入力に対する反応
		switch (state) {
		case State::NORMAL:
			charaEcpression = (int)Ecpression::NORMAL;
			SelectItem();
			break;
		case State::SELECT:
			if (manualAlpha < 1.0f) {
				manualAlpha += alphaSpeed;
			}
			charaEcpression = (int)Ecpression::NORMAL;
			SelectItem();
			SelectBuyNum();
			Buy();
			break;
		case State::BUY:
			charaEcpression = (int)Ecpression::SMILE;
			break;
		case State::BYE:
			charaEcpression = (int)Ecpression::SMILE;
			break;
		case State::NOT_MONEY:
			charaEcpression = (int)Ecpression::TROUBLE;
			break;
		}
		if (state != State::SELECT) {
			if (manualAlpha > 0.0f) {
				manualAlpha -= alphaSpeed;
			}
		}

		if (alpha < 1) {
			alpha += alphaSpeed;
		}

	}
	else {
		if (alpha > 0) {
			alpha -= alphaSpeed;
		}
		if (manualAlpha > 0.0f) {
			manualAlpha -= alphaSpeed;
		}
	}
	//描画するA値を設定
	shopBase.SetAlpha(alpha);
	for (int i = 0; i < itemValueNum; i++) {
		shopBaseLine[i].SetAlpha(alpha);
	}
	manualBase.SetAlpha(manualAlpha);

	textBase.SetAlpha(alpha);
	rightArrow.SetAlpha(alpha);
	leftArrow.SetAlpha(alpha);
	strColor.A = alpha;
	buyButton.SetAlpha(alpha);
}
void Shop::Draw() {
	Font* meirio=nullptr;
	meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);
	Font* akazukin = nullptr;
	akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);

	auto item = Item::GetInstance();
	//アイテムリストベース
	shopBase.Draw();
	for (int i = 0; i < itemValueNum; i++) {
		//アイテム下線
		shopBaseLine[i].Draw();
		//アイテム名
		float y = 340 + i * shopBaseLine[i].GetHeight();
		float scale = 1.8;

		meirio->DrawString(itemName[i], Vector2(1420, y), scale, strColor,true);

		char tmp[256];
		//アイテムの所持数
		sprintf_s(tmp, "%d", item->GetItemNum(itemName[i]));
		meirio->DrawString(tmp, Vector2(1650, y), scale, strColor,true);

		//アイテムの１つ当たりの金額
		sprintf_s(tmp, "%d", itemParam[i].price);
		meirio->DrawString(tmp, Vector2(1750, y), scale, strColor,true);

	}
	selectFrame.SetAll(Vector2(1575, 340 + selectNum * shopBaseLine[0].GetHeight()), Vector2(1, 1), 0, alpha);
	selectFrame.Draw();
	
	//文字列のベース
	textBase.Draw();
	//キャラの名前
	akazukin->DrawString(reactionParam[(int)state].name, Vector2(330, 800), 2.5, strColor, true);
	//キャラのテキスト
	meirio->DrawString(reactionParam[(int)state].str1, strPos, strFloat, strColor);

	//アイテム説明文ベース
	manualBase.Draw();	
	if (selectNum != -1) {
		Color manualStrColor = strColor;
		manualStrColor.A = manualAlpha;
		//アイテム説明文＿名前
		akazukin->DrawString(itemName[selectNum], Vector2(1000, 140), 2.5, manualStrColor, true);
		//アイテム説明文
		akazukin->DrawString(itemParam[selectNum].str1, Vector2(810, 500), 1.5, manualStrColor);
		akazukin->DrawString(itemParam[selectNum].str2, Vector2(810, 570), 1.5, manualStrColor);
	}
	switch (state) {
	case State::SELECT:

		//購入数
		char tmp[256];
		sprintf_s(tmp, "購入数　%dこ", buyNum);
		meirio->DrawString(tmp, Vector2(330, 945), 2, strColor);
		//購入金額
		sprintf_s(tmp, "合計金額　%dラウ", allPrice);
		meirio->DrawString(tmp, Vector2(580, 945), 2, strColor);

		//アイテム個数変更用矢印
		rightArrow.Draw();
		leftArrow.Draw();
		//購入ボタン
		buyButton.Draw();

		break;
	}


}