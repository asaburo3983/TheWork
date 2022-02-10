#pragma once
#include "LowApplication.h"
#include "Object.h"
#include "DXTK.h"
#include "FBX.h"
#include "EffekseerDX12.h"

#include "Shader.h"
class Resource {
public:
	static Resource* GetInstance() {
		static Resource rm;
		return &rm;
	}
private:
	std::unordered_map<std::string, dxtk::Image2D*> image;
	std::unordered_map<std::string, dxtk::Sound_MP3*> sound;
	std::unordered_map<std::string, fbx::FBX*> model;
	std::unordered_map<std::string, efk::Effect*> effect;
	std::unordered_map<std::string, shader::Shader*> shader;

public:
	void LoadIm(const char* _filepath);
	void LoadSound(const char* _filepath);
	void LoadModel(std::string _filepath, std::string _texpath);
	void LoadEffect(const char* _ufilepath);
	void LoadShader(shader::ShaderType _shaderType, const char* _filepath);

	void UnLoadIm(std::string _name);
	void UnLoadSound(std::string _name);
	void UnLoadModel(std::string _name);
	void UnLoadEffect(std::string _name);
	void UnLoadShader(std::string _name);

	dxtk::Image2D* Image(std::string _name);
	dxtk::Sound_MP3* Sound(std::string _name);
	fbx::FBX* Model(std::string _name);
	efk::Effect* Effect(std::string _name);
	shader::Shader* Shader(std::string _name);

	void AllDelete();
};
