#pragma once
#include "Object.h"
#include <shlobj.h>
#include "OpenFileDialog.h"
#include "Material.h"
#include "ModelObjectExtend.h"

#include "Resource.h"
#include "MaterialLinkerData.h"


class MaterialLinker {
public:
	ModelObjectExtend modelObj;

	char name[256]="Default";

	OpenFileDialog fbxOpenDialog;
	OpenFileDialog textureFolderOpenDialog;
	OpenFileDialog vertexShaderOpenDialog;
	OpenFileDialog pixelShaderOpenDialog;

	string modelPath;
	string texturePath;
	string shaderPathV;
	string shaderPathP;

	string modelPathOld;
	string texturePathOld;
	string shaderPathVOld;
	string shaderPathPOld;

	int meshMax = -1;
	int editMaterialNum = -1;

	
	std::vector<Material> material;

	char saveFolderName[256];

	MaterialLinker();
	void Manager();
	void DrawShadow();
	void DrawModel();

	void LoadMaterialLinker();
private:
	void SetResourcePath();
	void LoadModel();

	void SaveFile();
	void Save();
	//���b�V���̐��}�e���A���I�u�W�F�N�g�����݂���
	//MaterialObject materialObj[256];
	// 
	//���ׂĂɓK��������p�̐��l
	bool allOutlineLink = false;
	float allOutline;
	float allOutlineColor[3];

	MaterialLinkerData saveData;

};