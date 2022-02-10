#include "NormalUI.h"
#include "SoundManager.h"

void NormalUI::Initialize() {
	//画像の初期化と初期位置
	auto rc = Resource::GetInstance();

	dayBase.SetResource(rc->Image("DayBase.png"));
	dayBase.SetAll(Vector2(0,0));
	dayBase.SetCenter(false);

	moneyBase.SetResource(rc->Image("MoneyBase.png"));
	moneyBase.SetAll(Vector2(1650, 100), Vector2(1, 1), 0, 1);

	itemBase.SetResource(rc->Image("ItemBase.png"));
	itemBase.SetAll(Vector2(1575, 620));
	for (int i = 0; i < 7; i++) {
		itemLine[i].SetResource(rc->Image("ItemBaseLine.png"));
		int y = 340 + itemLine[i].GetHeight() * i;
		itemLine[i].SetAll(Vector2(1575, y));
	}
	manualBase.SetResource(rc->Image("ManualBase.png"));
	manualBase.SetAll(Vector2(1010, 410));

	selectFrame.SetResource(rc->Image("SelectFrame.png"));


	rightArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetTurn(true, false);
	rightArrow.SetAll(Vector2(1800, 960), Vector2(0.5, 0.5), 0, 0);
	leftArrow.SetAll(Vector2(1350, 960), Vector2(0.5, 0.5), 0, 0);

	items[0].SetResource(rc->Image("Maso_Seed.png"));
	items[1].SetResource(rc->Image("Tanepopo_Seed.png"));
	items[2].SetResource(rc->Image("Beranda_Seed.png"));
	items[3].SetResource(rc->Image("Panda_Seed.png"));
	items[4].SetResource(rc->Image("Rose_Seed.png"));
	items[5].SetResource(rc->Image("Sugger_Seed.png"));
	items[6].SetResource(rc->Image("Cosmo_Seed.png"));
	items[7].SetResource(rc->Image("Masomaso_Seed.png"));
	items[8].SetResource(rc->Image("Ranpopo_Seed.png"));
	items[9].SetResource(rc->Image("Barabara_Seed.png"));
	items[10].SetResource(rc->Image("SuggerPan_Seed.png"));
	items[11].SetResource(rc->Image("Frau_Seed.png"));
	items[12].SetResource(rc->Image("MagicPlanter.png"));
	items[13].SetResource(rc->Image("BugFix.png"));
	items[14].SetResource(rc->Image("Book0.png"));
	items[15].SetResource(rc->Image("Book1.png"));
	items[16].SetResource(rc->Image("Book2.png"));
	items[17].SetResource(rc->Image("Book3.png"));
	items[18].SetResource(rc->Image("Book4.png"));
}
void NormalUI::Update() {
	Item* item = Item::GetInstance();
	auto key = KeyInput::GetInstance();
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto novelSystem= NovelSystem::GetInstance();

	bool magicShop = MagicShop::GetInstance()->GetEnable();
	bool handyShop = HandyShop::GetInstance()->GetEnable();

	itemBase.SetAlpha(inventoryAlpha);
	for (int i = 0; i < 8; i++) {
		itemLine[i].SetAlpha(inventoryAlpha);
	}
	manualBase.SetAlpha(inventoryAlpha);

	selectFrame.SetAll(Vector2(1575, 340 + selectItemNum * itemLine[0].GetHeight()), Vector2(1, 1), 0, inventoryAlpha);
	rightArrow.SetAlpha(inventoryAlpha);
	leftArrow.SetAlpha(inventoryAlpha);

	//アイテムインベントリの描画
	if (key->key[DIK_E] == 1&& novelSystem->GetEnable()==false&&magicShop==false&&handyShop==false) {
		inventory = !inventory;
		selectItemNum = 0;
		sound->GetSE(SoundList_SE::ENTER)->Play();
	}
	//閉じるときだけ右クリックでもできる
	if (mouse->right == 1&& inventory == true) {
			inventory = false;
	}
	//別イベント時は表示しない
	if (novelSystem->GetEnable()|| magicShop || handyShop) {
		inventory = false;
	}
	//フェード処理
	if (inventory) {
		if (inventoryAlpha < 1.0f) {
			inventoryAlpha += inventoryAlphaSpeed;
		}
		if (mouse->left == 1) {
			for (int i = 0; i < 7; i++) {
				if (itemLine[i].Hit(mouse->x, mouse->y)) {
					selectItemNum = i;
					sound->GetSE(SoundList_SE::ENTER)->Play();
					break;
				}
			}
			if (rightArrow.Hit(mouse->x, mouse->y)) {
				page++;
				selectItemNum = 0;
				if (page > 3) {
					page = 1;
				}
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
			else if (leftArrow.Hit(mouse->x, mouse->y)) {
				page--;
				selectItemNum = 0;
				if (page < 1) {
					page = 3;
				}
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
		}

	}
	else {
		if (inventoryAlpha > 0.0f) {
			inventoryAlpha -= inventoryAlphaSpeed;
		}
	}
	

	
}
void NormalUI::Draw() {
	Item* item = Item::GetInstance();
	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);
	auto eventManager = EventManager::GetInstance();

	//お金ベース
	moneyBase.Draw();
	//お金金額　文字
	akazukin->DrawString(item->GetMoneyStr(), Vector2(1650, 75), 2.5, Color(255, 249, 187, 1), Vector2(1, 0));
	akazukin->DrawString("ラウ", Vector2(moneyBase.GetPos().X + 125, moneyBase.GetPos().Y), 3, Color(239, 159, 222, 1), true);

	//日付ベース
	dayBase.Draw();
	//日付　文字
	akazukin->DrawString("5", Vector2(80, 170), 6, Color(107, 72, 46, 1), true);
	char dayStr[50];
	sprintf_s(dayStr, "%d", eventManager->GetDays());
	if (eventManager->GetDays() > 9) {
		akazukin->DrawString(dayStr, Vector2(230, 230), 5, Color(107, 72, 46, 1), true);
	}
	else {
		akazukin->DrawString(dayStr, Vector2(200, 230), 5, Color(107, 72, 46, 1), true);
	}
	sprintf_s(dayStr, "のこり%d日", 31-eventManager->GetDays());
	akazukin->DrawString(dayStr, Vector2(170, 320), 2.5, Color(250, 120, 40, 1), true);

	if (inventoryAlpha > 0) {
		//アイテムベース
		itemBase.Draw();
		//アイテム　ライン
		for (int i = 0; i < 7; i++) {
			int num = i + (page - 1) * 7;
			if (num > 18) {
				break;
			}
			itemLine[i].Draw();
		}

		for (int i = 0; i < 7; i++) {
			int num = i + (page - 1) * 7;
			if (num > 18) {
				break;
			}
			//アイテム名前　文字
			float y = 340 + i * itemLine[0].GetHeight();
			if (item->GetItemGet(num)) {
				meirio->DrawString(item->GetItemName(num), Vector2(1420, y), 1.8, Color(63, 41, 20, inventoryAlpha), true);
			}
			else {
				meirio->DrawString("？？？", Vector2(1420, y), 1.8, Color(63, 41, 20, inventoryAlpha), true);
			}


			//アイテムの所持数
			char tmp[256];
			sprintf_s(tmp, "%d", item->GetItemNum(num));
			meirio->DrawString(tmp, Vector2(1750, y), 1.8, Color(63, 41, 20, inventoryAlpha), true);
		}

		//アイテム説明　ベース
		int itemNum = (page - 1) * 7 + selectItemNum;
		if (item->GetItemGet(itemNum)) {
			manualBase.Draw();
			//アイテム説明　アイテム
			items[itemNum].SetAll(Vector2(1015, 320), Vector2(1, 1), 0, inventoryAlpha);
			items[itemNum].Draw();
			//アイテム説明文字列
			Color manualStrColor(63.0f, 41.0f, 20.0f, inventoryAlpha);
			//アイテム説明文＿名前
			akazukin->DrawString(item->GetItemName(itemNum), Vector2(1000, 140), 2.5, manualStrColor, true);
			//アイテム説明文
			akazukin->DrawString(item->GetManual1(itemNum), Vector2(810, 500), 1.5, manualStrColor);
			akazukin->DrawString(item->GetManual2(itemNum), Vector2(810, 570), 1.5, manualStrColor);

		}
		//アイテム選択フレーム
		selectFrame.Draw();
		//ページ送りボタン
		rightArrow.Draw();
		leftArrow.Draw();
		//ページ数表示文字列
		char tmp[50];
		sprintf_s(tmp, "%d Page", page);
		akazukin->DrawString(tmp, Vector2(1550, 960), 2.2, Color(1, 1, 1, inventoryAlpha), true);
	}
}
