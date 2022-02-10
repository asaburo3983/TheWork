#define _CRT_SECURE_NO_WARNINGS
#include "EditList.h"
using namespace lowApp;

void EditList::Manager() {
	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();

	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO.X, winX * POS_RATIO.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO.X, winY * SIZE_RATIO.Y), 1);

	ImGui::Begin(u8"�}�e���A�������J�[���X�g");
	ImGui::Text(u8"�쐬����}�e���A�������J�[�̖��O");

	UpdateMaterial();

	AddMaterial();

	RemoveMaterial();

	ImGui::End();

	//�ʂ�GUI�ɕ����邽�߂ɂ����ŏ�������
	RemoveCheck();
	Warning();
}

void EditList::UpdateMaterial() {
	//�}�e���A�������J�[�̗L��,�����������܂�
	for (int i = 0; i < materialLinker.size(); i++) {

		ImGui::Checkbox(materialLinker[i].name, &inputEnableCache[i]);

		materialLinker[i].Manager();
	}
}
void EditList::AddMaterial() {
	ImGui::InputText(u8"", inputNameCache[materialLinker.size()], sizeof(inputNameCache[materialLinker.size()]));

	if (ImGui::Button(u8"�쐬")) {
			MaterialLinker materialTmp;

			//materialTmp.name = inputNameCache[materialLinker.size()];		

			if (strcmp(materialTmp.name, "") == 0) {
				return;  
			}
			for (int i = 0; i < materialLinker.size(); i++) {
				if (strcmp(materialTmp.name, materialLinker[i].name)==0) {
					//���O�������Ƃ��͍쐬���s��Ȃ�
					warning = true;
					return;
				}
			}			
			materialLinker.push_back(materialTmp);
			//materialLinker[materialLinker.size() - 1].name= inputNameCache[materialLinker.size()-1];
	}
}
void EditList::RemoveMaterial() {

	if (ImGui::Button(u8"�폜")) {
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

	//�}�e���A�����X�g�̍X�V
	ImGui::Begin(u8"�ŏI�m�F");
	for (int i = materialLinker.size() - 1; i >= 0; i--) {
		ImGui::Text(materialLinker[i].name);
	}
	ImGui::Text(u8"�ȏ�̃}�e���A�������J�[���폜���܂����H");
	if (ImGui::Button(u8"�͂�")) {
		for (int i = materialLinker.size()-1; i >= 0; i--) {

			materialLinker.erase(materialLinker.begin() + i);
			inputEnableCache[i] = false;
		}
		removeCheckEnable = false;
	}
	if (ImGui::Button(u8"������")) {
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

	//�}�e���A�����X�g�̍X�V
	ImGui::Begin(u8"�x��");
	ImGui::Text(u8"�������O�̃����J�[���쐬���Ȃ��ł�������");
	if (ImGui::Button(u8"�͂�")) {
		warning = false;
	}
	ImGui::End();
}