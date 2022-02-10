#include "Resource.h"

void Resource::LoadIm(const char* _filepath) {
	if (image[_filepath] != nullptr) {
		MessageBox(NULL, TEXT("画像データが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return;
	}
	image[_filepath] = new dxtk::Image2D();
	image[_filepath]->Load(_filepath);
}
void Resource::LoadSound(const char* _filepath) {
	if (sound[_filepath] != nullptr) {
		MessageBox(NULL, TEXT("サウンドデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return;
	}
	sound[_filepath] = new dxtk::Sound_MP3();
	sound[_filepath]->Load(_filepath);
}
void Resource::LoadModel(std::string _filepath, std::string _texpath) {
	if (model[_filepath] != nullptr) {
		MessageBox(NULL, TEXT("モデルデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return;
	}
	model[_filepath] = new fbx::FBX();
	model[_filepath]->Load(_filepath, _texpath);
}
void Resource::LoadEffect(const char* _filepath) {
	char16_t ustr[256];
	mbstate_t* ps=nullptr;
	mbrtoc16(ustr, _filepath, 256, ps);

	if (effect[_filepath] != nullptr) {
		MessageBox(NULL, TEXT("エフェクトデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return;
	}
	effect[_filepath] = new efk::Effect();
	effect[_filepath]->Load(ustr);
}
void Resource::LoadShader(shader::ShaderType _shaderType, const char* _filepath) {
	if (shader[_filepath] != nullptr) {
		MessageBox(NULL, TEXT("シェーダーデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return;
	}
	shader[_filepath] = new shader::Shader();
	shader[_filepath]->Load(_shaderType, _filepath);
}
void Resource::UnLoadIm(std::string _filepath) {
	auto itr = image.find(_filepath);
	if (itr != image.end())         // キーの要素が存在している場合
		image.erase(itr);
}
void Resource::UnLoadSound(std::string _filepath) {
	auto itr = sound.find(_filepath);
	if (itr != sound.end())         // キーの要素が存在している場合
		sound.erase(itr);
}
void Resource::UnLoadModel(std::string _filepath) {
	auto itr = model.find(_filepath);
	if (itr != model.end())         // キーの要素が存在している場合
		model.erase(itr);
}
void Resource::UnLoadEffect(std::string _filepath) {
	auto itr = effect.find(_filepath);
	if (itr != effect.end())         // キーの要素が存在している場合
		effect.erase(itr);
}
void Resource::UnLoadShader(std::string _filepath) {
	auto itr = shader.find(_filepath);
	if (itr != shader.end())         // キーの要素が存在している場合
		shader.erase(itr);
}
void Resource::AllDelete() {

	while (image.size() > 0) {
		auto itr = image.begin();
		image.erase(itr);
	}
	while (sound.size() > 0) {
		auto itr = sound.begin();
		sound.erase(itr);
	}
	while (model.size() > 0) {
		auto itr = model.begin();
		model.erase(itr);
	}
	while (effect.size() > 0) {
		auto itr = effect.begin();
		effect.erase(itr);
	}

}
dxtk::Image2D* Resource::Image(std::string _filepath) {
	return image[_filepath];
}
dxtk::Sound_MP3* Resource::Sound(std::string _filepath) {
	return sound[_filepath];
}
fbx::FBX* Resource::Model(std::string _filepath) {
	return model[_filepath];
}
efk::Effect* Resource::Effect(std::string _filepath) {
	return effect[_filepath];
}
shader::Shader* Resource::Shader(std::string _filepath) {
	return shader[_filepath];
}
