
#include "LowApplication.h"
using namespace frauEngine;


//imgui用の処理
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
void LowApplication::EnableDebugLayer() {
	if (!debugMode)return;

	ID3D12Debug* debugLayer = nullptr;
	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
		MessageBox(NULL, TEXT("デバッグレイヤーの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	debugLayer->EnableDebugLayer();
	debugLayer->Release();

}
void LowApplication::Init(const char* _winName, int _windowSizeX, int _windowSizeY, bool _fullscreen) {

	//以下を書いておかないとCOMが旨く動かずWICが正常に動作しないことがあります。
	if (FAILED(CoInitializeEx(0, COINIT_MULTITHREADED))) {
		MessageBox(NULL, TEXT("COMの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	
	EnableDebugLayer();

	InitWindow(_winName, _windowSizeX, _windowSizeY, _fullscreen);

	if (FAILED(InitDevice())) {
		MessageBox(NULL, TEXT("デバイスの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	if (FAILED(InitCommand())) {
		MessageBox(NULL, TEXT("コマンドリストの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	if (FAILED(InitSwapChain())) {
		MessageBox(NULL, TEXT("スワップチェインの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	if (FAILED(InitRenderTarget())) {
		MessageBox(NULL, TEXT("レンダーターゲットの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	if (FAILED(InitDepth())) {
		MessageBox(NULL, TEXT("深度バッファヒープの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}
	if (FAILED(InitFence())) {
		MessageBox(NULL, TEXT("フェンスの初期化に失敗しました"), TEXT("DeviceError"), MB_OK | MB_ICONERROR);
	}

	InitDefaultPipelineStateDesc();

	InitInput(windowClass.hInstance, hwnd);

	ShowWindow(hwnd, SW_SHOW);
}

void LowApplication::InitWindow(const char* winName, int _windowSizeX, int _windowSizeY, bool _fullscreen) {

	windowSizeX = _windowSizeX;
	windowSizeY = _windowSizeY;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	windowClass.lpszClassName = _T("DirectXTest");
	windowClass.hInstance = GetModuleHandle(0);
	//windowClass.hIcon= LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1), //アイコン
	RegisterClassEx(&windowClass);

	RECT wrc = { 0,0, windowSizeX,windowSizeY };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	
	DWORD fullscreenNum = (_fullscreen) ? WS_MAXIMIZE | WS_POPUP : (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE;
	// ウィンドウ作成
	hwnd = CreateWindowEx(
		//WS_EX_LAYERED,
		NULL,
		windowClass.lpszClassName,
		winName,//タイトルバーの文字
		fullscreenNum,//フルスクリーンとウィンドウの切り替え
		CW_USEDEFAULT,
		CW_USEDEFAULT,//表示Y座標はOSにお任せします
		wrc.right - wrc.left,//ウィンドウ幅
		wrc.bottom - wrc.top,//ウィンドウ高
		NULL,
		NULL,
		windowClass.hInstance,//呼び出しアプリケーションハンドル
		NULL);
}

HRESULT LowApplication::InitDevice() {
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;

	HRESULT result = S_OK;
	result= CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(&dxgiFactory));
	
	if (FAILED(result)) {
		return result;
	}
	
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	std::vector <IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}
	//NVIDIAグラフィクスボードがないとき別の物を指定
	if (tmpAdapter == nullptr) {
		for (auto adpt : adapters) {
			DXGI_ADAPTER_DESC adesc = {};
			adpt->GetDesc(&adesc);
			std::wstring strDesc = adesc.Description;
			tmpAdapter = adpt;
		}
	}
	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto l : levels) {
		if (D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(&device)) == S_OK) {
			featureLevel = l;
			result = S_OK;
			break;
		}
	}
	return result;
}

HRESULT LowApplication::InitCommand() {
	auto result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmmandAllocator));
	if (FAILED(result)) {
		return result;
	}
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmmandAllocator, nullptr, IID_PPV_ARGS(&cmmandList));
	if (FAILED(result)) {
		return result;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;//タイムアウトなし
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmmandQueue));
	if (FAILED(result)) {
		return result;
	}
	return S_OK;
}

HRESULT LowApplication::InitSwapChain() {
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = windowSizeX;
	swapchainDesc.Height = windowSizeY;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return  dxgiFactory->CreateSwapChainForHwnd(cmmandQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain);
}

HRESULT LowApplication::InitRenderTarget() {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビューなので当然RTV
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;//表裏の２つ
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//特に指定なし

	//デスクリプターデスクでデスクリプターを生成
	auto result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&renderTargetViewHeaps));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	backBufferResources.resize(swcDesc.BufferCount);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = renderTargetViewHeaps->GetCPUDescriptorHandleForHeapStart();

	//SRGBレンダーターゲットビュー設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//RTVを２枚作成
	for (size_t i = 0; i < swcDesc.BufferCount; ++i) {
		result = swapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&backBufferResources[i]));
		assert(SUCCEEDED(result));
		rtvDesc.Format = backBufferResources[i]->GetDesc().Format;
		device->CreateRenderTargetView(backBufferResources[i], &rtvDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	return S_OK;
}

HRESULT LowApplication::InitDepth() {

	//深度バッファの仕様
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = windowSizeX;
	depthResDesc.Height = windowSizeY;
	depthResDesc.DepthOrArraySize = 1;//テクスチャ配列でもないし3Dテクスチャでもない
	depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;//深度値書き込み用フォーマット
	depthResDesc.SampleDesc.Count = 1;//サンプルは1ピクセル当たり1つ
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//このバッファは深度ステンシルとして使用します
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.MipLevels = 1;
	//デプス用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//DEFAULTだから後はUNKNOWN
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//このクリアバリューが重要な意味を持つ
	D3D12_CLEAR_VALUE _depthClearValue = {};
	_depthClearValue.DepthStencil.Depth = 1.0f;//深さ１(最大値)でクリア
	_depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//32bit深度値としてクリア

	ID3D12Resource* depthBuffer = nullptr;
	auto result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //デプス書き込みに使用
		&_depthClearValue,
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) {
		return result;
	}

	//深度のためのデスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};//深度に使うという事がわかればいい
	dsvHeapDesc.NumDescriptors = 1;//深度ビュー2つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビューとして使う
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result)) {
		return result;
	}

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//デプス値に32bit使用
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;//フラグは特になし

	auto handle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthBuffer, &dsvDesc, handle);//カメラから見た深度

	//深度ヒープの作成 クラスのメンバ変数に入れてるだけ
	depthHeap = dsvHeap;

	//深度のシェーダーリソース用デスクリプションヒープの作成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&depthSRVHeap));

	D3D12_SHADER_RESOURCE_VIEW_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resDesc.Texture2D.MipLevels = 1;
	resDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(depthBuffer, &resDesc, depthSRVHeap->GetCPUDescriptorHandleForHeapStart());

	return result;
}

HRESULT LowApplication::InitFence() {
	fence = nullptr;
	fenceValue = 0;
	return  device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void LowApplication::InitDefaultPipelineStateDesc() {

	pipelineStateDesc.resize(1);

	//パイプラインのひな形を作成
	SetSampleMaskAsDefaultPipelineState();
	SetBlendStateAsDefaultPipelineState();
	SetRasterizerStateAsDefaultPipelineState();
	SetDepthStencilStateAsDefaultPipelineState();
	SetTopologyTypeAsDefaultPipelineState();
	SetRenderTargetsAsDefaultPipelineState();
	SetSampleDescAsDefaultPipelineState();
	SetViewportAsDefaultPipelineState();
}

void LowApplication::SetSampleMaskAsDefaultPipelineState() {

	pipelineStateDesc[(int)PipelineType::DEFAULT].SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
}
void LowApplication::SetBlendStateAsDefaultPipelineState() {

	pipelineStateDesc[(int)PipelineType::DEFAULT].BlendState.AlphaToCoverageEnable = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].BlendState.IndependentBlendEnable = false;
	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = true;

	renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	renderTargetBlendDesc.LogicOpEnable = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].BlendState.RenderTarget[0] = renderTargetBlendDesc;
}
void LowApplication::SetRasterizerStateAsDefaultPipelineState() {
	//ラスタライザー
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.MultisampleEnable = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//ポリの両面を描画
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.DepthClipEnable = true;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.FrontCounterClockwise = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.DepthBias = 1;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.DepthBiasClamp = 2;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.SlopeScaledDepthBias = 2;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.AntialiasedLineEnable = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.ForcedSampleCount = 0;
	pipelineStateDesc[(int)PipelineType::DEFAULT].RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

}
void LowApplication::SetDepthStencilStateAsDefaultPipelineState() {
	//デプスバッファ
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.DepthEnable = true;//深度バッファを使う
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//全て書き込み
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.StencilEnable = false;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.StencilReadMask = 0;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.StencilWriteMask = 0;

	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	pipelineStateDesc[(int)PipelineType::DEFAULT].DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	pipelineStateDesc[(int)PipelineType::DEFAULT].DSVFormat = DXGI_FORMAT_D32_FLOAT;

}
void LowApplication::SetTopologyTypeAsDefaultPipelineState() {
	//ポリゴンの変形（三角にするなど）
	pipelineStateDesc[(int)PipelineType::DEFAULT].IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//ストリップ時のカットなし
	pipelineStateDesc[(int)PipelineType::DEFAULT].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形で構成
}
void LowApplication::SetRenderTargetsAsDefaultPipelineState() {
	//レンダーターゲット
	pipelineStateDesc[(int)PipelineType::DEFAULT].NumRenderTargets = 1;//今は１つのみ
	pipelineStateDesc[(int)PipelineType::DEFAULT].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～1に正規化されたRGBA
}
void LowApplication::SetSampleDescAsDefaultPipelineState() {
	//サンプリング数の決定
	pipelineStateDesc[(int)PipelineType::DEFAULT].SampleDesc.Count = 1;//サンプリングは1ピクセルにつき１
	pipelineStateDesc[(int)PipelineType::DEFAULT].SampleDesc.Quality = 0;//クオリティは最低
}
void LowApplication::SetViewportAsDefaultPipelineState() {
	viewport.resize(1);
	scissorrect.resize(1);
	//ビューポートの設定
	viewport[(int)PipelineType::DEFAULT].Width = windowSizeX;//出力先の幅(ピクセル数)
	viewport[(int)PipelineType::DEFAULT].Height = windowSizeY;//出力先の高さ(ピクセル数)
	viewport[(int)PipelineType::DEFAULT].TopLeftX = 0;//出力先の左上座標X
	viewport[(int)PipelineType::DEFAULT].TopLeftY = 0;//出力先の左上座標Y
	viewport[(int)PipelineType::DEFAULT].MaxDepth = 1.0f;//深度最大値
	viewport[(int)PipelineType::DEFAULT].MinDepth = 0.0f;//深度最小値
	//描画部分の切り抜き
	scissorrect[(int)PipelineType::DEFAULT].top = 0;//切り抜き上座標
	scissorrect[(int)PipelineType::DEFAULT].left = 0;//切り抜き左座標
	scissorrect[(int)PipelineType::DEFAULT].right = windowSizeX;//切り抜き右座標
	scissorrect[(int)PipelineType::DEFAULT].bottom =windowSizeY;//切り抜き下座標			
}


void LowApplication::ChangeResourceState(D3D12_RESOURCE_STATES _before, D3D12_RESOURCE_STATES _after) {
	
	if (resourceStates != _after) {
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(backBufferResources[backBufferIndex], _before, _after);
		cmmandList->ResourceBarrier(1, &resourceBarrier);
		resourceStates = _after;
	}
}
void LowApplication::DrawStart() {
	backBufferIndex = swapchain->GetCurrentBackBufferIndex();

	//ChangeResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	auto rtvH = renderTargetViewHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(backBufferIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));//ポインタの位置をずらす
	auto dsvH = depthHeap->GetCPUDescriptorHandleForHeapStart();

	cmmandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
}
void LowApplication::DrawOnRenderTarget(ID3D12DescriptorHeap* _renderTargetBufferHeap) {

	ChangeResourceState(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);


	//レンダーターゲットを指定
	auto rtvH = _renderTargetBufferHeap->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(backBufferIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	auto dsvH = depthHeap->GetCPUDescriptorHandleForHeapStart();

	cmmandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmmandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	cmmandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	//ビューポート設定
	cmmandList->RSSetViewports(1, &viewport[(int)PipelineType::DEFAULT]);
	cmmandList->RSSetScissorRects(1, &scissorrect[(int)PipelineType::DEFAULT]);//描画範囲の設定

	//トポロジー設定
	cmmandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void LowApplication::DrawOnRenderTargetNoDepth(ID3D12DescriptorHeap* _renderTargetBufferHeap) {
	ChangeResourceState(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);


	//レンダーターゲットを指定
	auto rtvH = _renderTargetBufferHeap->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(backBufferIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	cmmandList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	cmmandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	//ビューポート設定
	cmmandList->RSSetViewports(1, &viewport[(int)PipelineType::DEFAULT]);
	cmmandList->RSSetScissorRects(1, &scissorrect[(int)PipelineType::DEFAULT]);//描画範囲の設定

	//トポロジー設定
	cmmandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void LowApplication::DrawOnScreen() {

	ChangeResourceState(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	auto rtvH = renderTargetViewHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(backBufferIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));//ポインタの位置をずらす

	auto dsvH = depthHeap->GetCPUDescriptorHandleForHeapStart();
	cmmandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmmandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmmandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
void LowApplication::DrawFinish() {

	

	ChangeResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	cmmandList->Close();

	ID3D12CommandList* cmdlists[] = { cmmandList };
	cmmandQueue->ExecuteCommandLists(1, cmdlists);

	cmmandQueue->Signal(fence, ++fenceValue);

	if (fence->GetCompletedValue() != fenceValue) {
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceValue, event);
		WaitForSingleObjectEx(event, INFINITE, false);
		CloseHandle(event);
	}
	if (FAILED(cmmandAllocator->Reset())) {
		MessageBox(NULL, TEXT("コマンドアロケーターのリセットに失敗しました"), TEXT("ResourceStateWarning"), MB_OK | MB_ICONERROR);
	}
	cmmandList->Reset(cmmandAllocator, nullptr);//再びコマンドリストをためる準備

	if (FAILED(swapchain->Present(1, 0))) {
		MessageBox(NULL, TEXT("画面のフリップに失敗しました"), TEXT("ResourceStateWarning"), MB_OK | MB_ICONERROR);
	}
}

void LowApplication::DrawOnDepth(ID3D12DescriptorHeap* depthBufferHeap,int texSize){

	auto dsvH = depthBufferHeap->GetCPUDescriptorHandleForHeapStart();

	cmmandList->OMSetRenderTargets(0, nullptr, false, &dsvH);
	cmmandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT viewportDepth;
	D3D12_RECT scissorrectDepth;

	viewportDepth.Width =  texSize;
	viewportDepth.Height = texSize;
	viewportDepth.TopLeftX = 0;
	viewportDepth.TopLeftY = 0;
	viewportDepth.MaxDepth = 1.0f;
	viewportDepth.MinDepth = 0.0f;

	scissorrectDepth.top    = 0;
	scissorrectDepth.left   = 0;
	scissorrectDepth.right  = texSize;
	scissorrectDepth.bottom = texSize;	

	cmmandList->RSSetViewports(1, &viewportDepth);
	cmmandList->RSSetScissorRects(1, &scissorrectDepth);//描画範囲の設定

	cmmandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


//終了処理
void LowApplication::UnInit() {

	if (dxgiFactory != nullptr) {
		dxgiFactory->Release();
		dxgiFactory = nullptr;
	}
	if (cmmandAllocator != nullptr) {
		cmmandAllocator->Release();
		cmmandAllocator = nullptr;
	}
	if (cmmandList != nullptr) {
		cmmandList->Release();
		cmmandList = nullptr;
	}
	if (cmmandQueue != nullptr) {
		cmmandQueue->Release();
		cmmandQueue = nullptr;
	}
	if (swapchain != nullptr) {
		swapchain->Release();
		swapchain = nullptr;
	}
	if (renderTargetViewHeaps != nullptr) {
		renderTargetViewHeaps->Release();
		renderTargetViewHeaps = nullptr;
	}
	while (backBufferResources.size() != 0) {
		backBufferResources.pop_back();
	}
	if (fence != nullptr) {
		fence->Release();
		fence = nullptr;
	}
	if (device != nullptr) {
		device->Release();
		device = nullptr;
	}

	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

