#define _CRT_SECURE_NO_WARNINGS
#include "BackObject.h"
using namespace lowApp;

BackObject::BackObject() {
	Resource* resource = Resource::GetInstance();

	modelPath = "Data/Sample/Back/Back.fbx";
	texturePath = "Data/Sample/Back/Tex/";
	
	shaderPathV = "Data/Shader/VertexShader.hlsl";
	shaderPathP = "Data/Shader/PixelShader.hlsl";

	modelPathOld = modelPath;
	texturePathOld = texturePath;
	shaderPathVOld = shaderPathV;
	shaderPathPOld = shaderPathP;

	resource->LoadModel(modelPath, texturePath);
//	resource->LoadShader(shader::VS, shaderPathV.c_str());
//  resource->LoadShader(shader::PS, shaderPathP.c_str());

	modelObj.Initialize(resource->Model(modelPath), resource->Shader(shaderPathV), resource->Shader(shaderPathP));

	modelObj.InitializeSub(ModelType::OUTLINE, resource->Shader(shaderPathV), resource->Shader("Data/Shader/PixelShader_Outline.hlsl"));
	modelObj.InitializeSub(ModelType::SHADOW, resource->Shader("Data/Shader/VertexShader_Shadow.hlsl"), resource->Shader("Data/Shader/PixelShader_Shadow.hlsl"));

	
}
void  BackObject::SetResourcePath() {
	ImGui::Text(modelPath.c_str());
	if (ImGui::Button("LoadModelPath")) {
		fbxOpenDialog.SetFbxPath();
		modelPath = fbxOpenDialog.GetFilePath();
	}
	ImGui::Text(texturePath.c_str());
	if (ImGui::Button("LoadTexturePath")) {
		textureFolderOpenDialog.SetFolderPath();
		texturePath = textureFolderOpenDialog.GetFilePath();
	}
	ImGui::Text(shaderPathV.c_str());
	if (ImGui::Button("LoadVertexShaderPath")) {
		vertexShaderOpenDialog.SetHLSLPath();
		shaderPathV = vertexShaderOpenDialog.GetFilePath();
	}
	ImGui::Text(shaderPathP.c_str());
	if (ImGui::Button("LoadPixelShaderPath")) {
		pixelShaderOpenDialog.SetHLSLPath();
		shaderPathP = pixelShaderOpenDialog.GetFilePath();
	}
}
void BackObject::LoadModel() {
	Resource* resource = Resource::GetInstance();

	if (ImGui::Button("LoadModel")) {

		resource->UnLoadModel(modelPathOld);
		resource->UnLoadShader(shaderPathVOld);
		resource->UnLoadShader(shaderPathPOld);

		resource->LoadModel(modelPath, texturePath);
		resource->LoadShader(shader::VS, shaderPathV.c_str());
		resource->LoadShader(shader::PS, shaderPathP.c_str());

		modelObj.Initialize(resource->Model(modelPath),
			resource->Shader(shaderPathV), resource->Shader(shaderPathP));
	}

}

void  BackObject::Manager() {
	static const Vector2 POS_RATIO = { 0,0 };
	static const Vector2 SIZE_RATIO = { 0.2,0.5 };

	Resource* resource = Resource::GetInstance();
	auto lowApp = LowApplication::GetInstance();
	auto winX = lowApp->GetWindowSizeX();
	auto winY = lowApp->GetWindowSizeY();

	ImGui::SetNextWindowPos(ImVec2(winX * POS_RATIO.X, winY * POS_RATIO.Y), 1);
	ImGui::SetNextWindowSize(ImVec2(winX * SIZE_RATIO.X, winY * SIZE_RATIO.Y), 1);

	ImGui::Begin(u8"背景オブジェクト");

	SetResourcePath();
	LoadModel();


	ImGui::End();

}

void BackObject::DrawShadow() {

	modelObj.SetAll(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	modelObj.SetAllAnimeState(false, 1, 0.2);

	modelObj.Draw(ModelType::SHADOW);
}
void  BackObject::DrawModel() {
	modelObj.SetAll(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
	modelObj.SetAllAnimeState(false, 1, 0.2);

	modelObj.Draw(ModelType::OUTLINE);
	modelObj.Draw(ModelType::MAIN);
}