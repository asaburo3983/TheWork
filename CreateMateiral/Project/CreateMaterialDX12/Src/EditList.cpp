#define _CRT_SECURE_NO_WARNINGS
#include "EditList.h"
using namespace lowApp;

void EditList::Manager() {
	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();

	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO.X, winX * POS_RATIO.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO.X, winY * SIZE_RATIO.Y), 1);

	ImGui::Begin(u8"マテリアルリンカーリスト");
	ImGui::Text(u8"作成するマテリアルリンカーの名前");

	UpdateMaterial();

	AddMaterial();

	RemoveMaterial();

	ImGui::End();

	//別のGUIに分けるためにここで処理する
	RemoveCheck();
	Warning();
}

void EditList::UpdateMaterial() {
	//マテリアルリンカーの有効,無効処理も含む
	for (int i = 0; i < materialLinker.size(); i++) {

		ImGui::Checkbox(materialLinker[i].name, &inputEnableCache[i]);

		materialLinker[i].Manager();
	}
}
void EditList::AddMaterial() {
	ImGui::InputText(u8"", inputNameCache[materialLinker.size()], sizeof(inputNameCache[materialLinker.size()]));

	if (ImGui::Button(u8"作成")) {
			MaterialLinker materialTmp;

			//materialTmp.name = inputNameCache[materialLinker.size()];		

			if (strcmp(materialTmp.name, "") == 0) {
				return;  
			}
			for (int i = 0; i < materialLinker.size(); i++) {
				if (strcmp(materialTmp.name, materialLinker[i].name)==0) {
					//名前が同じときは作成を行わない
					warning = true;
					return;
				}
			}			
			materialLinker.push_back(materialTmp);
			//materialLinker[materialLinker.size() - 1].name= inputNameCache[materialLinker.size()-1];
	}
}
void EditList::RemoveMaterial() {

	if (ImGui::Button(u8"削除")) {
		removeCheckEnable = true;
	}
}

void EditList::RemoveCheck() {
	if (!removeCheckEnable) return;

	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();
	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO_WARNING.X, winY * POS_RATIO_WARNING.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO_WARNING.X, winY * SIZE_RATIO_WARNING.Y), 1);
	ImGui::SetNextWindowFocus();

	//マテリアルリストの更新
	ImGui::Begin(u8"最終確認");
	for (int i = materialLinker.size() - 1; i >= 0; i--) {
		ImGui::Text(materialLinker[i].name);
	}
	ImGui::Text(u8"以上のマテリアルリンカーを削除しますか？");
	if (ImGui::Button(u8"はい")) {
		for (int i = materialLinker.size()-1; i >= 0; i--) {

			materialLinker.erase(materialLinker.begin() + i);
			inputEnableCache[i] = false;
		}
		removeCheckEnable = false;
	}
	if (ImGui::Button(u8"いいえ")) {
		removeCheckEnable = false;
	}
	ImGui::End();
}

void EditList::Warning() {
	if (!warning)return;

	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();
	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO_REMOVECHECK.X, winY * POS_RATIO_REMOVECHECK.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO_REMOVECHECK.X, winY * SIZE_RATIO_REMOVECHECK.Y), 1);
	ImGui::SetNextWindowFocus();

	//マテリアルリストの更新
	ImGui::Begin(u8"警告");
	ImGui::Text(u8"同じ名前のリンカーを作成しないでください");
	if (ImGui::Button(u8"はい")) {
		warning = false;
	}
	ImGui::End();
}