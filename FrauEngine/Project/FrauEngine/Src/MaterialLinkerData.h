#pragma once

#include "ModelObject.h"

namespace frauEngine {

	enum class MaterialSlot {
		DEFAULT = 50,
	};
	struct MaterialLinkerData {
	public:
		char materialPath[(int)MaterialSlot::DEFAULT][50];
	};
	struct MaterialData {
	public:
		//テクスチャの名前、出力用
		char anotherTexturePath[(int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER][32];

		bool anotherEnable[(int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER];

		//PBRステータス///////
		float subsurface = 0;

		float metallic = 0;

		float specular = 0;

		float specularTint = 0;

		float roufhness = 0;

		float anistropics = 0;

		float sheen = 0;

		float sheenTint = 0;

		float clearColor = 0;

		float clearcoatGloss = 0;

		float outline = 0;
		Vector3 outlineColor;

	};
	void LoadMaterialLinker(const char* _linkerPath, const char* _materialPath, const char* _texPath, ModelObject* _modelObj);
}