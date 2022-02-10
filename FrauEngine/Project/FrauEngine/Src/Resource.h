#pragma once
#include "LowApplication.h"
#include "Object.h"
#include "DXTK.h"
#include "FBX.h"
#include "EffekseerDX12.h"

#include "Shader.h"

namespace frauEngine {
	class Resource {
	public:
		static Resource* GetInstance() {
			static Resource rm;
			return &rm;
		}
	private:
		std::unordered_map<std::string, Image2D*> image;
		std::unordered_map<std::string, Sound_MP3*> sound;
		std::unordered_map<std::string, FBX*> model;
		std::unordered_map<std::string, Effect*> effect;
		std::unordered_map<std::string, Shader*> shader;

	public:
		Image2D* LoadIm(const char* _filepath);
		Sound_MP3* LoadSound(const char* _filepath);
		FBX* LoadModel(std::string _filepath, std::string _texpath);
		Effect* LoadEffect(const char* _ufilepath);
		Shader* LoadShader(frauEngine::ShaderType _shaderType, const char* _filepath);

		void UnLoadIm(std::string _name);
		void UnLoadSound(std::string _name);
		void UnLoadModel(std::string _name);
		void UnLoadEffect(std::string _name);
		void UnLoadShader(std::string _name);

		Image2D* Image(std::string _name);
		Sound_MP3* Sound(std::string _name);
		FBX* Model(std::string _name);
		Effect* Effect(std::string _name);
		Shader* Shader(std::string _name);

		void AllDelete();
	};
}