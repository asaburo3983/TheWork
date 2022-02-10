#include "Shader.h"
#include "Library.h"

using namespace Microsoft::WRL;
using namespace shader;

bool Shader::LoadCSO(const char* _filepath) {
	FILE* fp = nullptr;

	fopen_s(&fp, _filepath, "rb");
	if (fp == nullptr) {
		return false;
	}
	fseek(fp, 0, SEEK_END);
	BytecodeLength = ftell(fp);
	if (BytecodeLength == 0) {
		fclose(fp);
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	pShaderBytecode = new char[BytecodeLength];
	fread_s(pShaderBytecode, BytecodeLength, BytecodeLength, 1, fp);
	fclose(fp);

	shaderByteCode.BytecodeLength = BytecodeLength;
	shaderByteCode.pShaderBytecode = pShaderBytecode;
	return true;
}
bool Shader::LoadHLSL(ShaderType _shaderType, const char* _filepath) {

	

	std::wstring  wstr = GetWideStringFromString(_filepath);

	char shaderVarsion[5][7]{
		"vs_5_0","hs_5_0","ds_5_0","gs_5_0","ps_5_0"
	};
	auto result = D3DCompileFromFile(wstr.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", shaderVarsion[_shaderType-1], 0, 0,
		shaderBlob.ReleaseAndGetAddressOf(), NULL);
	if (FAILED(result)) {
		return false;
	}

	shaderByteCode = CD3DX12_SHADER_BYTECODE(shaderBlob.Get());
	return true;
}
bool Shader::Load(ShaderType _shaderType, const char* _filepath) {

	shaderType = _shaderType;

	if (strstr(_filepath, ".cso")) {
		if (LoadCSO(_filepath) == false) {
			MessageBox(NULL, TEXT("シェーダーの読み込みに失敗しました(CSOファイルが存在しません)"), TEXT("PathError"), MB_OK | MB_ICONERROR);
			return false;
		}
	}
	else if (strstr(_filepath, ".hlsl")) {
		if (LoadHLSL(_shaderType, _filepath) == false) {
			MessageBox(NULL, TEXT("シェーダーの読み込みに失敗しました(HLSLファイルが存在しません)"), TEXT("PathError"), MB_OK | MB_ICONERROR);
			return false;
		}
	}
	else {
		MessageBox(NULL, TEXT("シェーダーの読み込みに失敗しました（.csoまたは.hlslではない）"), TEXT("PathError"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;

}