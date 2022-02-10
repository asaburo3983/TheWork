#pragma once
#define _CRT_NO_SECURE_WARNINGS
#include "Application.h"
#include "Objects.h"

using namespace frauEngine;

class Item {
public:
	static Item* GetInstance() {
		static Item it;
		return &it;
	}

	void Initialize(std::string _filePath) {
		csvItem.LoadCSV(_filePath);

		for (int i = 0; i < 19; i++) {
			itemName[i] = csvItem.GetString(1 + i, 0);//名前
			item[itemName[i]];//定義しておく

			itemManual1[i] = csvItem.GetString(1 + i, 1);//説明文
			itemManual2[i] = csvItem.GetString(1 + i, 2);//説明文
			itemBug[i] = atoi(csvItem.GetString(1+i, 3).c_str());//虫
			itemNotBug[i] = atoi(csvItem.GetString(1+i, 4).c_str());//虫付きにくさ
			itemSell[i] = atoi(csvItem.GetString(1+i, 5).c_str());//売値
		}

	}
	void AddMoney(int _add) {
		money += _add;

	} 
	int GetMoney() {
		return money;
	}
	void SetMoney(int _money) {
		money = _money;
	}
	std::string GetMoneyStr() {
		return moneyStr;
	}
	void AddItem(std::string str, int _add) {
		item[str] += _add;
		//TODO　itemをマップで持つのはひどい処理なので余裕のある時に直す
		for (int i = 0; i < 19; i++) {
			int tmp = GetItemNum(i);
			if (tmp > 0) {
				itemGet[i] = true;
			}
		}
	}
	int GetItemNum(std::string _str) {
		return item[_str];
	}
	int GetItemNum(int _num) {	
		return item[itemName[_num]];
	}
	void SetItemNum(int _num, int _itemNum) {
		item[itemName[_num]] = _itemNum;
	}
	std::string GetItemName(int _num) {
		return itemName[_num];
	}

	void Update();

	int GetItemListSize() {
		return item.size();
	}
	string GetManual1(int _num) {
		return itemManual1[_num];
	}
	string GetManual2(int _num) {
		return itemManual2[_num];
	}
	int GetBug(int _num) {
		return itemBug[_num];
	}
	int GetNotBug(int _num) {
		return itemNotBug[_num];
	}
	int GetSell(int _num) {
		return itemSell[_num];
	}
	bool GetItemGet(int _num) {
		return itemGet[_num];
	}
	void SetItemGet(int _num, bool _get) {
		itemGet[_num] = _get;
	}
private:
	CSV csvItem;
	std::unordered_map<std::string, int> item;

	std::string itemName[19];
	std::string itemManual1[19];
	std::string itemManual2[19];
	int itemBug[19];
	int itemNotBug[19];
	int itemSell[19];
	bool itemGet[19];
	//金額表示で利用
	const int maxMoneyDigits = 8;
	int moneySurface = 1000;
	int money = 1000;
	std::string moneyStr = "00001000";
public:
	void Reset() {
		for (int i = 0; i < 19; i++) {
			item[GetItemName(i)] = 0;
		}
		moneySurface = 1000;
		money = 1000;
		moneyStr = "00001000";
	}

};
