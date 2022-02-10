#pragma once
#include "ModelObject.h"

enum class ModelType {
	MAIN,
	SHADOW,
	OUTLINE,
	MAX
};
class ModelObjectExtend : public obj::ModelObject {
private:
	
	obj::ModelObject modelObject[(int)ModelType::MAX - 1];

	Vector3 outlienScale = Vector3(1.001f, 1.001f, 1.001f);

public:

	void InitializeSub(ModelType _modelType,
		shader::Shader* _shaderV = nullptr,
		shader::Shader* _shaderP = nullptr,
		shader::Shader* _shaderG = nullptr,
		shader::Shader* _shaderH = nullptr,
		shader::Shader* _shaderD = nullptr
	);

	void SetOutlineScale(Vector3 _scale) { outlienScale = _scale; };

	void Draw(ModelType _modelType);

};