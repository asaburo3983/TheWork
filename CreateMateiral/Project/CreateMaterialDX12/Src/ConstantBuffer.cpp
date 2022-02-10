#include "ConstantBuffer.h"

using namespace cBuffer;
using namespace lowApp;

void ConstantBufferHeap::CreateConstantBufferHeap(int dataSize,void** data) {
	auto device = LowApplication::GetInstance()->GetDevice();

	//定数バッファの書き込み
	ID3D12Resource* constBuff = nullptr;
	D3D12_HEAP_PROPERTIES tmpProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC tmpDesc = CD3DX12_RESOURCE_DESC::Buffer((dataSize + 0xff) & ~0xff);
	//GetDeviceRemove

	auto result = device->CreateCommittedResource(
		&tmpProp,
		D3D12_HEAP_FLAG_NONE,
		&tmpDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);

	constBuff->Map(0, nullptr, data);//マップ
	constBuff->Unmap(0, nullptr);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NodeMask = 0;//マスクは0
	descHeapDesc.NumDescriptors = 1;//
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descripterHeap));//生成

	////デスクリプタの先頭ハンドルを取得しておく
	auto handle = descripterHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(constBuff->GetDesc().Width);

	//定数バッファビューの作成
	device->CreateConstantBufferView(&cbvDesc, handle);

}

void WorldBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(WorldBuffer), (void**)&buffer);
}
void BoneBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(BoneBuffer), (void**)&buffer);
}
void MaterialBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(MaterialBuffer), (void**)&buffer);
}
void CameraBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(CameraBuffer), (void**)&buffer);
}
void LightBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(LightBuffer), (void**)&buffer);
}

void UtilityBufferHeap::Create() {
	CreateConstantBufferHeap(sizeof(UtilityBuffer), (void**)&buffer);
}
void PBRBufferHeap::Create() {

	CreateConstantBufferHeap(sizeof(PBRBuffer), (void**)&buffer);

}
void PostEffectBufferHeap::Create() {

	CreateConstantBufferHeap(sizeof(PostEffectBuffer), (void**)&buffer);
}