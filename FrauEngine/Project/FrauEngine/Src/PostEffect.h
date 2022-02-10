#pragma once
#include "LowApplication.h"
#include "Shader.h"
#include "ConstantBuffer.h"
using namespace Microsoft::WRL;
namespace frauEngine {
	enum class PostEffectConstant {
		VALUE_SLOT = 1,
		TEXTURE_SLOT = 3
	};
	enum class AddPostEffect {
		NONE,
		DEPTHOFFIELD,
		BLUR,
		SHARP,
		RETORO,
		NEGA,
		OUTLINE,
		OUTLINE_DEPTHOFFIELD,
		MAX,
	};
	class PostEffect {
	public:
		void Create(
			frauEngine::Shader* _vs = nullptr,
			frauEngine::Shader* _ps = nullptr,
			frauEngine::Shader* _gs = nullptr,
			frauEngine::Shader* _hs = nullptr,
			frauEngine::Shader* _ds = nullptr);
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
		void SetColorPlus(Color _color) { constantBufferHeap.buffer->colorPlus = _color; }
		void SetColorMulti(Color _color) { constantBufferHeap.buffer->colorMulti = _color; }

	public:
		frauEngine::PostEffectBufferHeap constantBufferHeap;
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

		frauEngine::Shader* vs;
		frauEngine::Shader* ps;
		frauEngine::Shader* gs;
		frauEngine::Shader* hs;
		frauEngine::Shader* ds;

	};
}