#include "Resource.h"

using namespace frauEngine;

Image2D* Resource::LoadIm(const char* _filepath) {

	string strEnd = GetSplitEnd(_filepath);

	if (image[strEnd] != nullptr) {
		MessageBox(NULL, TEXT("画像データが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return nullptr;
	}
	image[strEnd] = new frauEngine::Image2D();
	image[strEnd]->Load(_filepath);

	return image[strEnd];
}
Sound_MP3* Resource::LoadSound(const char* _filepath) {

	string strEnd = GetSplitEnd(_filepath);

	if (sound[strEnd] != nullptr) {
		MessageBox(NULL, TEXT("サウンドデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return nullptr;
	}
	sound[strEnd] = new frauEngine::Sound_MP3();
	sound[strEnd]->Load(_filepath);

	return sound[strEnd];
}
FBX* Resource::LoadModel(std::string _filepath, std::string _texpath) {

	string strEnd = GetSplitEnd(_filepath);

	if (model[strEnd] != nullptr) {
		MessageBox(NULL, TEXT("モデルデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return nullptr;
	}
	model[strEnd] = new frauEngine::FBX();
	model[strEnd]->Load(_filepath, _texpath);

	return model[strEnd];
}
Effect* Resource::LoadEffect(const char* _filepath) {
	char16_t ustr[256];
	mbstate_t* ps=nullptr;
	mbrtoc16(ustr, _filepath, 256, ps);

	string strEnd = GetSplitEnd(_filepath);
	if (effect[strEnd] != nullptr) {
		MessageBox(NULL, TEXT("エフェクトデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return nullptr;
	}
	effect[strEnd] = new frauEngine::Effect();
	effect[strEnd]->Load(ustr);

	return effect[strEnd];
}
Shader* Resource::LoadShader(frauEngine::ShaderType _shaderType, const char* _filepath) {

	string strEnd = GetSplitEnd(_filepath);

	if (shader[strEnd] != nullptr) {
		MessageBox(NULL, TEXT("シェーダーデータが重複しています"), TEXT("ResourceError"), MB_OK | MB_ICONERROR);
		return nullptr;
	}
	shader[strEnd] = new frauEngine::Shader();
	shader[strEnd]->Load(_shaderType, _filepath);

	return shader[strEnd];
}
void Resource::UnLoadIm(std::string _filepath) {
	auto itr = image.find(_filepath);
	if (itr != image.end()) {         // キーの要素が存在している場合
		delete image[_filepath];
		image.erase(itr);
	}
}
void Resource::UnLoadSound(std::string _filepath) {
	auto itr = sound.find(_filepath);
	if (itr != sound.end()) {
		delete sound[_filepath];
		sound.erase(itr);
	}
}
void Resource::UnLoadModel(std::string _filepath) {
	auto itr = model.find(_filepath);
	if (itr != model.end()) {
		delete model[_filepath];
		model.erase(itr);
	}
}
void Resource::UnLoadEffect(std::string _filepath) {
	auto itr = effect.find(_filepath);
	if (itr != effect.end()) {
		delete effect[_filepath];
		effect.erase(itr);
	}
}
void Resource::UnLoadShader(std::string _filepath) {
	auto itr = shader.find(_filepath);
	if (itr != shader.end()) {
		delete shader[_filepath];
		shader.erase(itr);
	}
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
Image2D* Resource::Image(std::string _filepath) {
	return image[_filepath];
}
Sound_MP3* Resource::Sound(std::string _filepath) {
	return sound[_filepath];
}
FBX* Resource::Model(std::string _filepath) {
	return model[_filepath];
}
Effect* Resource::Effect(std::string _filepath) {
	return effect[_filepath];
}
Shader* Resource::Shader(std::string _filepath) {
	return shader[_filepath];
}
