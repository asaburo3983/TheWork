#include "ModelObjectExtend.h"

void ModelObjectExtend::InitializeSub(ModelType _modelType,
	shader::Shader* _shaderV,
	shader::Shader* _shaderP,
	shader::Shader* _shaderG,
	shader::Shader* _shaderH,
	shader::Shader* _shaderD
) {
	switch (_modelType) {
	case ModelType::SHADOW:
		modelObject[(int)_modelType - 1].Initialize(resource,
			_shaderV,
			_shaderP,
			_shaderG,
			_shaderH,
			_shaderD
		);
		break;
	case ModelType::OUTLINE:
		modelObject[(int)_modelType - 1].Initialize(resource,
			_shaderV,
			_shaderP,
			_shaderG,
			_shaderH,
			_shaderD,
			D3D12_CULL_MODE_FRONT
		);
		break;

	}


}

void ModelObjectExtend::Draw(ModelType _modelType) {

	int objectNum = (int)_modelType - 1;
	switch (_modelType) {
	case ModelType::MAIN:
		ModelObject::Draw();
		break;
	case ModelType::SHADOW:
		modelObject[objectNum].SetImGuiEnable(false);
		modelObject[objectNum].SetAll(pos, angle, scale);
		modelObject[objectNum].SetAllAnimeState(anime, animeNum, animeSpeed);
		modelObject[objectNum].Draw();
		break;
	case ModelType::OUTLINE:
		modelObject[objectNum].SetImGuiEnable(false);
		modelObject[objectNum].SetAll(pos, angle, scale);
		modelObject[objectNum].SetAllAnimeState(anime, animeNum, animeSpeed);
		modelObject[objectNum].SetScale(scale * outlienScale);
		modelObject[objectNum].Draw();
		break;
	}
}
