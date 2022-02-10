#pragma once
#include "ModelObject.h"

namespace frauEngine {
	enum class ModelType {
		MAIN,
		SHADOW,
		MAX
	};
	class ModelExtendObject : public ModelObject {
	private:

		ModelObject modelObject[(int)ModelType::MAX - 1];

		Vector3 outlienScale = Vector3(1.001f, 1.001f, 1.001f);

	public:

		void InitializeSub(ModelType _modelType,
			frauEngine::Shader* _shaderV = nullptr,
			frauEngine::Shader* _shaderP = nullptr,
			frauEngine::Shader* _shaderG = nullptr,
			frauEngine::Shader* _shaderH = nullptr,
			frauEngine::Shader* _shaderD = nullptr,
			D3D12_CULL_MODE _cullMode = D3D12_CULL_MODE_NONE
		);

		void SetOutlineScale(Vector3 _scale) { outlienScale = _scale; };

		void Draw(ModelType _modelType);

		ModelObject* GetModelObject(ModelType _modelType) {
			return &modelObject[(int)_modelType - 1];
		}
	};
}