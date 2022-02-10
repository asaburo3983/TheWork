#pragma once
#include "Object.h"
#include "MaterialLinkerData.h"
#include "OpenFileDialog.h"
#include "Shader.h"

#include "ModelObjectExtend.h"
using namespace shader;
class Material {
private:
	ModelObjectExtend* modelObj;
	int num;
	char name[32] = "DefaultMaterial";
	
	
	OpenFileDialog anotherTextureOpenDialog[(int)TextureSlot::MODEL_ANOTHER];

	bool loadedTexture[(int)TextureSlot::MODEL_ANOTHER];


	MaterialData saveData;
	float outlineColor[3];
	bool enable = true;
public:
	Material();
	void Setup(ModelObjectExtend* _modelObj, int _num);
	void Manager();
	void Save(string _folderPath);

	char* GetName() { return name; };

	
	void SetEnable(bool _enable) { enable = _enable; }
	bool GetEnable() { return enable; }

	void SetOutlineParam(float _outline, Vector3 _outlineColor) {
		saveData.outline = _outline;
		outlineColor[0] = _outlineColor.X;
		outlineColor[1] = _outlineColor.Y;
		outlineColor[2] = _outlineColor.Z;

	}

	void SetParam();
private:
	void SetTexture();

	void SetParameterPBR();

	
};