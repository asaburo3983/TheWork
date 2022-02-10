#define _CRT_SECURE_NO_WARNINGS
#include "MaterialLinker.h"
using namespace lowApp;

MaterialLinker::MaterialLinker() {
	Resource* resource = Resource::GetInstance();

	if (0) {
		modelPath = "Data/Model/Sample/Sample.fbx";
		texturePath = "Data/Model/Sample/Tex/";
	}
	else {
		modelPath = "Data/Model/Frau/Frau.fbx";
		texturePath = "Data/Model/Frau/Tex/";
	}
	shaderPathV = "Data/Shader/VertexShader.hlsl";
	shaderPathP = "Data/Shader/PixelShader.hlsl";
	
	modelPathOld = modelPath;
	texturePathOld = texturePath;
	shaderPathVOld = shaderPathV;
	shaderPathPOld = shaderPathP;

	resource->LoadModel(modelPath, texturePath);
	resource->LoadShader(shader::VS, shaderPathV.c_str());
	resource->LoadShader(shader::PS, shaderPathP.c_str());

	resource->LoadShader(shader::PS, "Data/Shader/PixelShader_Outline.hlsl");

	resource->LoadShader(shader::VS, "Data/Shader/VertexShader_Shadow.hlsl");
	resource->LoadShader(shader::PS, "Data/Shader/PixelShader_Shadow.hlsl");

	modelObj.Initialize(resource->Model(modelPath),
		resource->Shader(shaderPathV), resource->Shader(shaderPathP));

	modelObj.InitializeSub(ModelType::SHADOW, resource->Shader(shaderPathV), resource->Shader(shaderPathP));
	modelObj.InitializeSub(ModelType::OUTLINE, resource->Shader(shaderPathV), resource->Shader("Data/Shader/PixelShader_Outline.hlsl"));
	modelObj.InitializeSub(ModelType::SHADOW, resource->Shader("Data/Shader/VertexShader_Shadow.hlsl"), resource->Shader("Data/Shader/PixelShader_Shadow.hlsl"));

	meshMax = resource->Model(modelPath)->GetMeshMax() - 1;//���b�V������Ԃ��@�z��̊֌W�� -1
	material.resize(meshMax + 1);
	for (int i = 0; i < meshMax + 1; i++) {
		material[i].Setup(&modelObj, i);
	}

}
void MaterialLinker::SetResourcePath(){
	ImGui::Text(modelPath.c_str());
	if (ImGui::Button("LoadModelPath")) {
		fbxOpenDialog.SetFbxPath();
		modelPath = fbxOpenDialog.GetFilePath();
	}
	ImGui::Text(texturePath.c_str());
	if (ImGui::Button("LoadTexturePath")) {
		textureFolderOpenDialog.SetFolderPath();
		texturePath = textureFolderOpenDialog.GetFilePath();
	}
	ImGui::Text(shaderPathV.c_str());
	if (ImGui::Button("LoadVertexShaderPath")) {
		vertexShaderOpenDialog.SetHLSLPath();
		shaderPathV = vertexShaderOpenDialog.GetFilePath();
	}
	ImGui::Text(shaderPathP.c_str());
	if (ImGui::Button("LoadPixelShaderPath")) {
		pixelShaderOpenDialog.SetHLSLPath();
		shaderPathP = pixelShaderOpenDialog.GetFilePath();
	}


}
void MaterialLinker::LoadModel() {
	Resource* resource = Resource::GetInstance();
	
	if (ImGui::Button("LoadModel")) {

		resource->UnLoadModel(modelPathOld);
		resource->UnLoadShader(shaderPathVOld);
		resource->UnLoadShader(shaderPathPOld);

		resource->LoadModel(modelPath, texturePath);
		resource->LoadShader(shader::VS, shaderPathV.c_str());
		resource->LoadShader(shader::PS, shaderPathP.c_str());

		modelObj.Initialize(resource->Model(modelPath),
			resource->Shader(shaderPathV), resource->Shader(shaderPathP));

		meshMax = resource->Model(modelPath)->GetMeshMax() - 1;//���b�V������Ԃ��@�z��̊֌W�� -1
		material.resize(meshMax + 1);
		for (int i = 0; i < meshMax + 1; i++) {
			material[i].Setup(&modelObj, i);
		}

	}

}

void MaterialLinker::SaveFile(){

	//�}�e���A��������
	for (int i = 0; i < meshMax + 1; i++) {


		char* tmp = material[i].GetName();
		string str = tmp;
		//�g���q������
		if (str.find(".mat") == std::string::npos) {
			strcat(tmp, ".mat");
		}
		sprintf(saveData.materialPath[i], tmp);

		material[i].Save(saveFolderName);
	}
	//�}�e���A�������J�[���쐬���ĕۑ�
	string str = name;
	string str2 = "MaterialLinker/" + str + "/" + str;

	char filePath[256];
	sprintf(filePath, str2.c_str());
	strcat(filePath, ".matlink");

	FILE* fp = fopen(filePath, "wb");//�o�C�i���t�@�C�����J��

	fwrite(&saveData, sizeof(saveData), 1, fp); // SaveData_t�\���̂̒��g���o��
	fclose(fp);//�t�@�C�������
}
void MaterialLinker::Save() {
	//�Z�[�u�p�̃t�H���_�̍쐬
	TCHAR currentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentPath);

	
	sprintf_s(saveFolderName, currentPath);

	//�������Ƃ̃t�H���_�����
	strcat(saveFolderName, "/MaterialLinker/");
	_mkdir(saveFolderName);

	string strTmp = name;
	strTmp += "/";
	strcat(saveFolderName, strTmp.c_str());
	if (_mkdir(saveFolderName) == 0) {
		strcat(saveFolderName, "Material/");//�}�e���A���ۑ��p�̃f�B���N�g�����쐬���Ă���
		_mkdir(saveFolderName);
		SaveFile();

	}
	else {
		int id =MessageBox(NULL, TEXT("�Z�[�u�f�[�^�̏㏑�������܂���?"), TEXT("�Z�[�u�m�F"), MB_OKCANCEL | MB_ICONWARNING);

		switch (id)
		{
		case IDOK:
			strcat(saveFolderName, "Material/");//�}�e���A���ۑ��p�̃f�B���N�g�����쐬���Ă���
			_mkdir(saveFolderName);
			SaveFile();
			break;
		case IDCANCEL:
			return;
			break;
		}
	}

	string tmp = "�Ƀf�[�^��ۑ����܂���";
	string str = saveFolderName + tmp;
	MessageBox(NULL, TEXT(str.c_str()), TEXT("�Z�[�u����"), MB_OK | MB_ICONASTERISK);

}
void MaterialLinker::Manager() {
	static const Vector2 POS_RATIO = { 0.8,0 };
	static const Vector2 SIZE_RATIO = { 0.2,0.5 };

	Resource* resource = Resource::GetInstance();
	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();

	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO.X, winY * POS_RATIO.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO.X, winY * SIZE_RATIO.Y), 1);

	ImGui::Begin(u8"�}�e���A�������J�[");

	ImGui::InputText(u8"���O", name, sizeof(name));

	SetResourcePath();
	LoadModel();


	////���b�V���ԍ��̐ݒ�
	ImGui::SliderInt("EditMaterialNum", &editMaterialNum, -1, meshMax);
	ImGui::InputInt("", &editMaterialNum);
	if (editMaterialNum > meshMax) {
		editMaterialNum = meshMax;
	}
	else if (editMaterialNum < -1) {
		editMaterialNum = -1;
	}
	//���ׂẴA�E�g���C���ݒ���ψ�ɂ���
	ImGui::Checkbox(u8"�A�E�g���C���p�����[�^�����ׂẴ}�e���A���ɓK��", &allOutlineLink);
	if (allOutlineLink) {
		ImGui::SliderFloat("AllOutline", &allOutline, 0.0f, 1.0f);
		ImGui::ColorEdit3("AllOutlineColor", allOutlineColor);
		Vector3 outlineColorPack;
		outlineColorPack.X = allOutlineColor[0];
		outlineColorPack.Y = allOutlineColor[1];
		outlineColorPack.Z = allOutlineColor[2];
		for (int i = 0; i < meshMax; i++) {
			material[i].SetOutlineParam(allOutline, outlineColorPack);
		}
	}

	//
	if (ImGui::Button("SaveLinker")) {
		Save();
	}
	ImGui::End();

	//�}�e���A���̏���
	//�ҏW���ł͂Ȃ��}�e���A���𔼓����ɂ��鏈��
	for (int i = 0; i < meshMax; i++) {

		//�������s���}�e���A�����I������Ă��Ȃ��΂����������ɂ���
		if (editMaterialNum != i) {
			material[i].SetEnable(false);
		}
		else {
			material[i].SetEnable(true);
		}
		//�}�e���A���̑I�����Ȃ��Ƃ��͔����������͂Ȃ�
		if (editMaterialNum == -1) {
			material[i].SetEnable(true);
			material[i].Manager();
		}
		else {
			if (editMaterialNum == i) {
				material[i].Manager();
			}
		}
		material[i].SetParam();
		
	}

}
void MaterialLinker::DrawShadow() {
	modelObj.SetAll(Vector3(0, 0, 0), Vector3(-90, 180, 0), Vector3(3, 3, 3));
	modelObj.SetAllAnimeState(false, 1, 0.2);

	modelObj.Draw(ModelType::SHADOW);
}
void MaterialLinker::DrawModel() {
		modelObj.SetAll(Vector3(0, 0, 0), Vector3(-90, 180, 0), Vector3(3, 3, 3));
		modelObj.SetAllAnimeState(false, 1, 0.2);
		
	
		modelObj.Draw(ModelType::MAIN);
		modelObj.SetOutlineScale(Vector3(1.003f, 1.001f, 1.003f));
		//modelObj.Draw(ModelType::OUTLINE);
}