#define _CRT_SECURE_NO_WARNINGS
#include "Save.h"

void Save::SaveData() {
	//データを構造体に代入
	saveData.day = EventManager::GetInstance()->GetDays();
	auto item = Item::GetInstance();
	saveData.money = item->GetMoney();
	for (int i = 0; i < 19; i++) {
		saveData.itemNum[i] = item->GetItemNum(i);
		saveData.itemGetFlag[i] = item->GetItemGet(i);


	}
	for (int i = 0; i < (int)ScenarioName::MAX; i++) {
		saveData.novelFlag[i] = NovelSystem::GetInstance()->GetEnd((ScenarioName)i);
	}
	//データを書き込む
	FILE* fp = fopen("Data/Save/Save.dat", "wb");
	if (fp == NULL) {
		return;
	}
	fwrite(&saveData, sizeof(SaveDatas), 1, fp);
	fclose(fp);

}
void Save::LoadData() {

	//読み込み処理
	FILE* fp = fopen("Data/Save/Save.dat", "rb");
	if (fp == NULL) {
		return;
	}
	fread(&saveData, sizeof(SaveDatas), 1, fp);
	fclose(fp);

	//データを各種に代入
	EventManager::GetInstance()->SetDays(saveData.day);
	auto item = Item::GetInstance();
	item->SetMoney(saveData.money);

	for (int i = 0; i < 19; i++) {
		item->SetItemNum(i, saveData.itemNum[i]);

		item->SetItemGet(i, saveData.itemGetFlag[i]);
	}
	for (int i = 0; i < (int)ScenarioName::MAX; i++) {
		saveData.novelFlag[i] = NovelSystem::GetInstance()->GetEnd((ScenarioName)i);
		NovelSystem::GetInstance()->SetStart((ScenarioName)i, saveData.novelFlag[i]);
		NovelSystem::GetInstance()->SetEnd((ScenarioName)i, saveData.novelFlag[i]);
	}
}