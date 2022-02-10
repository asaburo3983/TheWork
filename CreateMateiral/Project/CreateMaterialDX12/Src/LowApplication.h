#pragma once
#include "Library.h"


LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
namespace lowApp {
	enum class PipelineType {
		DEFAULT = 0,
	};
	class LowApplication {
	public:
		static LowApplication* GetInstance()
		{
			static LowApplication lapp;
			return &lapp;
		}

		void Init(const char* _winName, int _windowSizeX, int _windowSizeY, bool _fullscreen);
		void UnInit();

		//描画系統


		void DrawStart();

		void DrawOnRenderTarget(ID3D12DescriptorHeap* _renderTargetBufferHeap);
		void DrawOnRenderTargetNoDepth(ID3D12DescriptorHeap* _renderTargetBufferHeap);

		void DrawOnScreen();

		void DrawOnDepth(ID3D12DescriptorHeap* depthBufferHeap, int texSize);

		void DrawFinish();

		


		HWND* GetHWND() { return &hwnd; };
		ID3D12Device* GetDevice() { return device; };
		ID3D12CommandQueue* GetCommandQueue() { return cmmandQueue; };
		ID3D12GraphicsCommandList* GetCommandList() { return cmmandList; };
		int GetWindowSizeX() { return windowSizeX; };
		int GetWindowSizeY() { return windowSizeY; };
		D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPipelineStateDesc(PipelineType _pipelineType) { return pipelineStateDesc[(int)_pipelineType]; };
		D3D12_VIEWPORT GetViewPort(PipelineType _pipelineType) { return viewport[(int)_pipelineType]; };

		std::vector<ID3D12Resource*> GetBackBuffers() { return backBufferResources; };
		ID3D12DescriptorHeap* GetRenderTargetHeaps() { return renderTargetViewHeaps; };

		const float* GetClearColor() { return clearColor; };

		void SetDebugMode(bool _debugMode) { debugMode = _debugMode; };
		bool GetDebugMode() { return debugMode; };

		ID3D12DescriptorHeap* GetDepthResource() { return depthSRVHeap; };
	private:
		void EnableDebugLayer();
		void InitWindow(const char* _winName, int _windowSizeX, int _windowSizeY, bool _fullscreen);
		HRESULT InitDevice();
		HRESULT InitCommand();
		HRESULT InitSwapChain();
		HRESULT InitRenderTarget();
		HRESULT InitDepth();
		HRESULT InitFence();

		void InitDefaultPipelineStateDesc();
		void SetSampleMaskAsDefaultPipelineState();
		void SetBlendStateAsDefaultPipelineState();
		void SetRasterizerStateAsDefaultPipelineState();
		void SetDepthStencilStateAsDefaultPipelineState();
		void SetTopologyTypeAsDefaultPipelineState();
		void SetRenderTargetsAsDefaultPipelineState();
		void SetSampleDescAsDefaultPipelineState();
		void SetViewportAsDefaultPipelineState();

		void ChangeResourceState(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after);
	private:
		HWND hwnd;
		WNDCLASSEX windowClass = {};
		int windowSizeX, windowSizeY;

		IDXGIFactory4* dxgiFactory = nullptr;
		ID3D12Device* device = nullptr;

		ID3D12CommandAllocator* cmmandAllocator = nullptr;
		ID3D12GraphicsCommandList* cmmandList = nullptr;
		ID3D12CommandQueue* cmmandQueue = nullptr;

		IDXGISwapChain4* swapchain = nullptr;

		std::vector<ID3D12Resource*> backBufferResources;

		ID3D12DescriptorHeap* renderTargetViewHeaps = nullptr;

		ID3D12DescriptorHeap* depthHeap;
		ID3D12DescriptorHeap* depthSRVHeap;

		ID3D12Fence* fence = nullptr;
		UINT64 fenceValue = 0;

		std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC> pipelineStateDesc;
		std::vector<D3D12_VIEWPORT> viewport;
		std::vector<D3D12_RECT> scissorrect;

		UINT backBufferIndex;
		D3D12_RESOURCE_BARRIER resourceBarrier;
		D3D12_RESOURCE_STATES resourceStates;

		const float clearColor[4] = { 0.0f,1.0f,0.0f,1.0f };//グリーン

		bool debugMode = true;//読み込み負荷のためにリリースモードでも

	};
}