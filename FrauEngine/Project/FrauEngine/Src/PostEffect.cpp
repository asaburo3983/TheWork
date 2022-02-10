#include "PostEffect.h"

using namespace frauEngine;

void PostEffect::Create(
	frauEngine::Shader* _vs,
	frauEngine::Shader* _ps,
	frauEngine::Shader* _gs,
	frauEngine::Shader* _hs,
	frauEngine::Shader* _ds){

	vs = _vs;
	ps = _ps;
	gs = _gs;
	hs = _hs;
	ds = _ds;

	CreateRenderTarget();
	CreatePolygon();
	CreatePipeline();

	constantBufferHeap.Create();

	for (int i = 0; i < (int)PostEffectConstant::TEXTURE_SLOT; i++) {
		resourceHeap[i] = nullptr;
	}

	SetMainTexture(registerHeap);
}
void PostEffect::CreateRenderTarget() {
	auto lowApp = LowApplication::GetInstance();
	auto device = lowApp->GetDevice();
	auto backBuffers = lowApp->GetBackBuffers();
	auto rtvDescHeap = lowApp->GetRenderTargetHeaps();
	const float* clearColor = lowApp->GetClearColor();
		
	auto& bbuff = backBuffers[0];
	auto resDesc = bbuff->GetDesc();//���Ƃ��Ǝg���Ă���o�b�N�o�b�t�@�̏��𗘗p����
	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
	auto result = device->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	result = device->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(resource2.ReleaseAndGetAddressOf()));

	if (FAILED(result)) {
		MessageBox(NULL, TEXT("���\�[�X�̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}

	auto heapDesc = rtvDescHeap->GetDesc();
	heapDesc.NumDescriptors = 2;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(renderTargetViewHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("RTV�f�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	auto handle = renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();

	device->CreateRenderTargetView(resource.Get(), &rtvDesc, handle);
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	device->CreateRenderTargetView(resource2.Get(), &rtvDesc, handle);

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;

	//�V�F�[�_�[�ɓn���p��DescripterHeap�����ASRV�̉摜�f�[�^�������Ă�
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(registerHeap.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("SRV�q�[�v�̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}
	handle = registerHeap->GetCPUDescriptorHandleForHeapStart();



	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//1����
	device->CreateShaderResourceView(resource.Get(), &srvDesc, handle);
	//2����
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	device->CreateShaderResourceView(resource2.Get(), &srvDesc, handle);


	return;
}
void PostEffect::CreatePolygon() {
	auto lowApp = LowApplication::GetInstance();
	auto device = lowApp->GetDevice();

	struct PeraVertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
	PeraVertex pv[4] = { {{-1,-1,0.1},{0,1}},
						{{-1,1,0.1},{0,0}},
						{{1,-1,0.1},{1,1}},
						{{1,1,0.1},{1,0}} };

	CD3DX12_HEAP_PROPERTIES hp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC rd = CD3DX12_RESOURCE_DESC::Buffer(sizeof(pv));
	auto result = device->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&rd,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("���_�o�b�t�@�̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}

	//�}�b�v����
	PeraVertex* mappedPera = nullptr;
	vertexBuffer->Map(0, nullptr, (void**)&mappedPera);
	std::copy(std::begin(pv), std::end(pv), mappedPera);
	vertexBuffer->Unmap(0, nullptr);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(pv);
	vertexBufferView.StrideInBytes = sizeof(PeraVertex);
}
void PostEffect::CreatePipeline() {
	auto lowApp = LowApplication::GetInstance();
	auto device = lowApp->GetDevice();

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	const int valueSlot = (int)PostEffectConstant::VALUE_SLOT;
	const int textureSlot = (int)PostEffectConstant::TEXTURE_SLOT;
	const int allSlot = (int)PostEffectConstant::VALUE_SLOT + (int)PostEffectConstant::TEXTURE_SLOT;

	D3D12_DESCRIPTOR_RANGE range[allSlot] = {};
	for (int i = 0; i < valueSlot; i++) {
		range[i].NumDescriptors = 1;
		range[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//t
		range[i].BaseShaderRegister = i;//���W�X�^�͂O
		range[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}
	for (int i = valueSlot; i < allSlot; i++) {
		range[i].NumDescriptors = 1;
		range[i].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//t
		range[i].BaseShaderRegister = i;
		range[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3D12_ROOT_PARAMETER rp[allSlot] = {};
	for (int i = 0; i < allSlot; i++) {
		rp[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//
		rp[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rp[i].DescriptorTable.pDescriptorRanges = &range[i];
		rp[i].DescriptorTable.NumDescriptorRanges = 1;
	}

	rsDesc.pParameters = rp;//���[�g�p�����[�^�̐擪�A�h���X
	rsDesc.NumParameters = allSlot;//���[�g�p�����[�^��

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//��Ԃ��Ȃ�(�j�A���X�g�l�C�o�[)
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɍ��i
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂݉�

	rsDesc.pStaticSamplers = &samplerDesc;
	rsDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rsBlob;
	ComPtr<ID3DBlob> errBlob;
	auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, rsBlob.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	result = device->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		MessageBox(NULL, TEXT("���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};


	gpsDesc.VS = vs->GetShaderByteCode();
	gpsDesc.PS = ps->GetShaderByteCode();
	if (gs != nullptr) {
		gpsDesc.GS = gs->GetShaderByteCode();
	}
	if (ds != nullptr) {
		gpsDesc.DS = ds->GetShaderByteCode();
	}
	if (hs != nullptr) {
		gpsDesc.HS = hs->GetShaderByteCode();
	}

	gpsDesc.DepthStencilState.DepthEnable = false;
	gpsDesc.DepthStencilState.StencilEnable = false;
	D3D12_INPUT_ELEMENT_DESC layout[2] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};
	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;
	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	gpsDesc.pRootSignature = rootSignature.Get();

	result = device->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));

	if (FAILED(result)) {
		MessageBox(NULL, TEXT("�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���"), TEXT("PostEffectError"), MB_OK | MB_ICONERROR);
	}
}

void PostEffect::Draw() {
	auto lowApp = LowApplication::GetInstance();
	auto deice = lowApp->GetDevice();
	auto cmdList = lowApp->GetCommandList();

	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);

	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	
	int textureStartNum = (int)PostEffectConstant::VALUE_SLOT;

	//�萔�o�b�t�@�𑗂�
	cmdList->SetDescriptorHeaps(1, &constantBufferHeap.descripterHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, constantBufferHeap.descripterHeap->GetGPUDescriptorHandleForHeapStart());

	//�`�悵���e�N�X�`�����͂�
	cmdList->SetDescriptorHeaps(1, resourceHeap[0].GetAddressOf());
	cmdList->SetGraphicsRootDescriptorTable(textureStartNum, resourceHeap[0]->GetGPUDescriptorHandleForHeapStart());

	//�[�x�e�N�X�`���𑗂�
	auto depthResourceHeap = lowApp->GetDepthResource();
	cmdList->SetDescriptorHeaps(1, &depthResourceHeap);
	cmdList->SetGraphicsRootDescriptorTable(textureStartNum + 1, depthResourceHeap->GetGPUDescriptorHandleForHeapStart());

	//�u���[�p�e�N�X�`���𑗂�
	if (resourceHeap[2] != nullptr) {
		cmdList->SetDescriptorHeaps(1, resourceHeap[2].GetAddressOf());
		cmdList->SetGraphicsRootDescriptorTable(textureStartNum + 2, resourceHeap[2]->GetGPUDescriptorHandleForHeapStart());
	}

	cmdList->DrawInstanced(4, 1, 0, 0);
}
