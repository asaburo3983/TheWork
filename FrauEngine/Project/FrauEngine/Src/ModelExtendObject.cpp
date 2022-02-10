#include "ModelExtendObject.h"

using namespace frauEngine;

void ModelExtendObject::InitializeSub(ModelType _modelType,
	frauEngine::Shader* _shaderV,
	frauEngine::Shader* _shaderP,
	frauEngine::Shader* _shaderG,
	frauEngine::Shader* _shaderH,
	frauEngine::Shader* _shaderD,
	D3D12_CULL_MODE _cullMode
) {
	switch (_modelType) {
	case ModelType::SHADOW:
		modelObject[(int)_modelType - 1].Initialize(resource,
			_shaderV,
			_shaderP,
			_shaderG,
			_shaderH,
			_shaderD,
			_cullMode
		);
		break;
	//case ModelType::OUTLINE:
	//	modelObject[(int)_modelType - 1].Initialize(resource,
	//		_shaderV,
	//		_shaderP,
	//		_shaderG,
	//		_shaderH,
	//		_shaderD,

	//		D3D12_CULL_MODE_FRONT
	//	);
	//case ModelType::SUB1:
	////case ModelType::SUB2:
	////case ModelType::SUB3:
	//	modelObject[(int)_modelType - 1].Initialize(resource,
	//		_shaderV,
	//		_shaderP,
	//		_shaderG,
	//		_shaderH,
	//		_shaderD,
	//		_cullMode
	//	);
	//	break;

	}


}

void ModelExtendObject::Draw(ModelType _modelType) {

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
	//case ModelType::OUTLINE:
	//	modelObject[objectNum].SetImGuiEnable(false);
	//	modelObject[objectNum].SetAll(pos, angle, scale);
	//	modelObject[objectNum].SetAllAnimeState(anime, animeNum, animeSpeed);
	//	modelObject[objectNum].SetScale(scale * outlienScale);
	//	modelObject[objectNum].Draw();
	//	break;
	//case ModelType::SUB1:
	//case ModelType::SUB2:
	//case ModelType::SUB3:
		//modelObject[objectNum].SetImGuiEnable(false);
		//modelObject[objectNum].SetAll(pos, angle, scale);
		//modelObject[objectNum].SetAllAnimeState(anime, animeNum, animeSpeed);
		//modelObject[objectNum].Draw();
		//break;
	}
}
