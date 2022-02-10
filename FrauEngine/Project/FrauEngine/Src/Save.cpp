#define _CRT_SECURE_NO_WARNINGS
#include "Save.h"

void Save::SaveData() {
	//�f�[�^���\���̂ɑ��
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
	//�f�[�^����������
	FILE* fp = fopen("Data/Save/Save.dat", "wb");
	if (fp == NULL) {
		return;
	}
	fwrite(&saveData, sizeof(SaveDatas), 1, fp);
	fclose(fp);

}
void Save::LoadData() {

	//�ǂݍ��ݏ���
	FILE* fp = fopen("Data/Save/Save.dat", "rb");
	if (fp == NULL) {
		return;
	}
	fread(&saveData, sizeof(SaveDatas), 1, fp);
	fclose(fp);

	//�f�[�^���e��ɑ��
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