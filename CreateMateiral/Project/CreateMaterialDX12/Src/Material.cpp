#define _CRT_SECURE_NO_WARNINGS

#include "Material.h"

using namespace lowApp;

string anotherTextureSlotName[(int)TextureSlot::MODEL_ANOTHER];

Material::Material() {
}

void Material::Setup(ModelObjectExtend* _modelObj, int _num){

	string str = "material" + to_string(_num);
	sprintf_s(name, str.c_str());

	modelObj = _modelObj; num = _num; 
	for (int i = 0; i < (int)TextureSlot::MODEL_ANOTHER; i++) {
		anotherTextureSlotName[i] = "anotherTexture" + to_string(i);

		loadedTexture[i] = false;

		saveData.anotherEnable[i] = false;

		ZeroMemory(&saveData.anotherTexturePath[i], sizeof(saveData.anotherTexturePath[i]));
	}

	

	ZeroMemory(&anotherTextureOpenDialog, sizeof(anotherTextureOpenDialog));
};
void Material::SetTexture() {
	for (int i = 0; i < (int)TextureSlot::MODEL_ANOTHER; i++) {

		char imguiStrTmp[256];
		sprintf_s(imguiStrTmp, "Enable%d", i);

		if (loadedTexture[i]) {
			ImGui::Checkbox(imguiStrTmp, &saveData.anotherEnable[i]);
		}

		ImGui::Text(anotherTextureSlotName[i].c_str());

		ImGui::Text(anotherTextureOpenDialog[i].GetFilePath());

		sprintf_s(imguiStrTmp, "TextureSelect%d", i);
		if (ImGui::Button(imguiStrTmp)) {
			anotherTextureOpenDialog[i].SetTexturePath();
		}

		modelObj->pbrBufferHeap[num].buffer->anotherTexture[i] = saveData.anotherEnable[i];

		sprintf_s(imguiStrTmp, "TextureLoad%d", i);
		if (ImGui::Button(imguiStrTmp)) {
			
			modelObj->SetAnotherTexture(num, anotherTextureOpenDialog[i].GetFilePath(), i);
			
			saveData.anotherEnable[i] = true;
			loadedTexture[i] = true;

			char buffer[256];
			ZeroMemory(buffer, sizeof(char) * 256);
			memcpy(buffer, anotherTextureOpenDialog[i].GetFilePath(), sizeof(char) * 256);
			Replace('\\', '/', buffer);
			std::vector<std::string> split_list;
			std::string replace_file_name = buffer;
			Split('/', buffer, split_list);

			sprintf_s(saveData.anotherTexturePath[i], split_list[split_list.size() - 1].c_str());
		}

	}
}
void Material::SetParameterPBR() {

	ImGui::SliderFloat("Subsurface", &saveData.subsurface, 0.0f, 1.0f);
	ImGui::SliderFloat("Metallic", &saveData.metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &saveData.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("SpecularTint", &saveData.specularTint, 0.0f, 1.0f);
	ImGui::SliderFloat("Roufhness", &saveData.roufhness, 0.0f, 1.0f);
	ImGui::SliderFloat("Anistropics", &saveData.anistropics, 0.0f, 1.0f);
	ImGui::SliderFloat("Sheen", &saveData.sheen, 0.0f, 1.0f);
	ImGui::SliderFloat("SheenTint", &saveData.sheenTint, 0.0f, 1.0f);
	ImGui::SliderFloat("ClearColor", &saveData.clearColor, 0.0f, 1.0f);
	ImGui::SliderFloat("ClearcoatGloss", &saveData.clearcoatGloss, 0.0f, 1.0f);

	ImGui::SliderFloat("Outline", &saveData.outline, 0.0f, 1.0f);
	
	ImGui::ColorEdit3("OutlineColor", outlineColor);

	saveData.outlineColor.X = outlineColor[0];
	saveData.outlineColor.Y = outlineColor[1];
	saveData.outlineColor.Z = outlineColor[2];

	float workSpace = 0.5f;
	if (enable) {
		workSpace = 1.0f;
	}
	
	modelObj->SetPBR(
		num,
		saveData.subsurface,
		saveData.metallic,
		saveData.specular,
		saveData.specularTint,
		saveData.roufhness,
		saveData.anistropics,
		saveData.sheen,
		saveData.sheenTint,
		saveData.clearColor,
		saveData.clearcoatGloss,
		saveData.outline,
		saveData.outlineColor,
		workSpace
	);
}

void Material::Save(string _folderPath) {

	//TODO マテリアルのセーブ機能をつける

		string filePath = _folderPath;
		string str = name;
		filePath += name;

		if (filePath.find(".mat") == std::string::npos) {
			filePath += ".mat";
		}
		
		FILE* fp = fopen(filePath.c_str(), "wb");//バイナリファイルを開く

		fwrite(&saveData, sizeof(saveData), 1, fp); // SaveData_t構造体の中身を出力
		fclose(fp);//ファイルを閉じる
	
}
void Material::Manager() {

	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();

	static const Vector2 POS_RATIO = { 0.8,0.5 };
	static const Vector2 SIZE_RATIO = { 0.2,0.5 };

	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO.X, winY * POS_RATIO.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO.X, winY * SIZE_RATIO.Y), 1);

	ImGui::Begin(u8"マテリアル");

	ImGui::InputText(u8"名前", name, sizeof(name));

	SetParameterPBR();

	SetTexture();
	
	//マテリアルの個別セーブは意味がないので行わない
	//マテリアルリンカーのセーブ時にまとめてマテリアルのセーブを行う
	ImGui::End();
}
void Material::SetParam() {
	saveData.outlineColor.X = outlineColor[0];
	saveData.outlineColor.Y = outlineColor[1];
	saveData.outlineColor.Z = outlineColor[2];

	float workSpace = 0.5f;
	if (enable) {
		workSpace = 1.0f;
	}

	modelObj->SetPBR(
		num,
		saveData.subsurface,
		saveData.metallic,
		saveData.specular,
		saveData.specularTint,
		saveData.roufhness,
		saveData.anistropics,
		saveData.sheen,
		saveData.sheenTint,
		saveData.clearColor,
		saveData.clearcoatGloss,
		saveData.outline,
		saveData.outlineColor,
		workSpace
	);
}