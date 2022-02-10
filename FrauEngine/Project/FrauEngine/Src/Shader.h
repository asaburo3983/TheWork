#pragma once
#include "Library.h"
using namespace Microsoft::WRL;
namespace frauEngine {
	enum class ShaderTextureSlot {
		MODEL_DEFAULT = 2,
		MODEL_ANOTHER = 8

	};
	enum ShaderType {
		NONE, VS, HS, DS, GS, PS
	};
	class Shader {
	private:
		ComPtr<ID3DBlob> shaderBlob;
		ShaderType shaderType;
		D3D12_SHADER_BYTECODE shaderByteCode;
		char* pShaderBytecode;//cso�t�@�C������̓ǂݍ��ݗp
		long BytecodeLength;
	private:
		bool LoadCSO(const char* _filepath);
		bool LoadHLSL(ShaderType _shaderType, const char* _filepath);
	public:
		
		bool Load(ShaderType _shaderType, const char* _filepath);

		D3D12_SHADER_BYTECODE GetShaderByteCode() { return shaderByteCode; };
		ShaderType GetShaderType() { return shaderType; };
	};
}