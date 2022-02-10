#pragma once
#include "Object.h"
#include <shlobj.h>
#include "OpenFileDialog.h"
#include "Material.h"
#include "ModelObjectExtend.h"

#include "Resource.h"

class  BackObject {
public:
	ModelObjectExtend modelObj;

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


	BackObject();
	void Manager();
	void DrawShadow();
	void DrawModel();
private:
	void SetResourcePath();
	void LoadModel();


};