#pragma once
#include "LowApplication.h"

namespace srBuffer {
	class ShaderResourceBufferHeap {
	private:
		using LoadTextureLambda = std::function<HRESULT(const std::wstring path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
		std::map<std::string, LoadTextureLambda> loadTextureLambda;
	public:
		ShaderResourceBufferHeap();
		ID3D12DescriptorHeap* descripterHeap = nullptr;
		void CreateShaderResourceBufferHeap(ID3D12Resource* resource);//リソースから生成する

		void CreateShaderResourceBufferHeap(std::string _texturePath);//ファイルパスから作成する
	};
}