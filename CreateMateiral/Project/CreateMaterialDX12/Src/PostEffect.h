#pragma once
#include "LowApplication.h"
#include "Shader.h"
#include "ConstantBuffer.h"
using namespace Microsoft::WRL;

enum class PostEffectConstant {
	VALUE_SLOT = 1,
	TEXTURE_SLOT = 3
};

class PostEffect {
public:
	void Create(
		shader::Shader* _vs = nullptr,
		shader::Shader* _ps = nullptr,
		shader::Shader* _gs = nullptr,
		shader::Shader* _hs = nullptr,
		shader::Shader* _ds = nullptr);
	void Draw();

	void SetMainTexture(ComPtr<ID3D12DescriptorHeap> _resourceHeap) {
		resourceHeap[0] = _resourceHeap;
	}
	void SetBlurTexture(ComPtr<ID3D12DescriptorHeap> _resourceHeap) {
		resourceHeap[2] = _resourceHeap;
	}
	ComPtr<ID3D12DescriptorHeap> resourceHeap[(int)PostEffectConstant::TEXTURE_SLOT];

	ComPtr<ID3D12Resource> GetResource() { return resource; };
	ComPtr<ID3D12DescriptorHeap> GetRenderTargetHeap() { return renderTargetViewHeap; };
	ComPtr<ID3D12DescriptorHeap> GetResourceHeap() { return registerHeap; };

public:
	cBuffer::PostEffectBufferHeap constantBufferHeap;
private:
	void CreateRenderTarget();
    void CreatePolygon();
    void CreatePipeline();
private:

	ComPtr<ID3D12DescriptorHeap> renderTargetViewHeap;
	ComPtr<ID3D12DescriptorHeap> registerHeap;
	ComPtr<ID3D12Resource> resource;
	ComPtr<ID3D12Resource> resource2;

	ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	ComPtr<ID3D12RootSignature> rootSignature;

	ComPtr<ID3D12PipelineState> pipelineState;

	shader::Shader* vs;
	shader::Shader* ps;
	shader::Shader* gs;
	shader::Shader* hs;
	shader::Shader* ds;

};