#include "Light.h"

using namespace frauEngine;

Lights::Lights() {
	//深度情報を作る
	
	auto device = LowApplication::GetInstance()->GetDevice();

	//定数バッファの作成
	lightBufferHeap.Create();

	//深度バッファの仕様
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2次元のテクスチャデータとして
	depthResDesc.Width = shadowTexSize;//幅と高さはレンダーターゲットと同じ
	depthResDesc.Height =shadowTexSize;//上に同じ
	depthResDesc.DepthOrArraySize = 1;//テクスチャ配列でもないし3Dテクスチャでもない
	depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;//深度値書き込み用フォーマット
	depthResDesc.SampleDesc.Count = 1;//サンプルは1ピクセル当たり1つ
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//このバッファは深度ステンシルとして使用します
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.MipLevels = 1;
	//デプス用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//DEFAULTだから後はUNKNOWNでよし
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


	//深度のためのデスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};//深度に使うよという事がわかればいい
	dsvHeapDesc.NumDescriptors = 1;//深度ビュー
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビューとして使う
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depthHeap));



	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//デプス値に32bit使用
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;//フラグは特になし

	auto handle = depthHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthBuffer, &dsvDesc, handle);//カメラから見た深度


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

	handle = depthSRVHeap->GetCPUDescriptorHandleForHeapStart();
	//カメラから見たデプステクスチャ
	device->CreateShaderResourceView(depthBuffer, &resDesc, handle);

	//環境光の初期値を入れておく
	lightBufferHeap.buffer->ambientLight.x = 0.5f;
}

void Lights::SetAmbientLight(float _power) {
	lightBufferHeap.buffer->ambientLight.x = _power;
}
void Lights::SetDirectionalLight(Vector3 _pos, Vector3 _target, Vector3 _up, float _fov, Vector3 _vector, Vector3 _color) {

	lightBufferHeap.buffer->directionalLightPos.x = _pos.X;
	lightBufferHeap.buffer->directionalLightPos.y = _pos.Y;
	lightBufferHeap.buffer->directionalLightPos.z = _pos.Z;
	lightBufferHeap.buffer->directionalLightColor.x = _color.X;
	lightBufferHeap.buffer->directionalLightColor.y = _color.Y;
	lightBufferHeap.buffer->directionalLightColor.z = _color.Z;
	lightBufferHeap.buffer->directionalLightVector.x = _vector.X;
	lightBufferHeap.buffer->directionalLightVector.y = _vector.Y;
	lightBufferHeap.buffer->directionalLightVector.z = _vector.Z;

	XMFLOAT3 pos;
	pos.x = _pos.X;
	pos.y = _pos.Y;
	pos.z = _pos.Z;
	XMFLOAT3 up;
	up.x = _up.X;
	up.y = _up.Y;
	up.z = _up.Z;
	XMFLOAT3 target;
	target.x = _target.X;
	target.y = _target.Y;
	target.z = _target.Z;

	lightBufferHeap.buffer->directionalLightView = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
	lightBufferHeap.buffer->directionalLightProjection = XMMatrixPerspectiveFovLH(45,
		static_cast<float>(LowApplication::GetInstance()->GetWindowSizeX()) /
		static_cast<float>(LowApplication::GetInstance()->GetWindowSizeY()),
		0.1f,//近い方
		1000.0f//遠い方
	);
}

void Lights::SetPointLight(int _lightNum, Vector3 _pos, float _range, Vector3 _color) {
	if (_lightNum >= frauEngine::POINT_LIGHT) {
		MessageBox(NULL, TEXT("ライトの数が多すぎます、定数が足りていません"), TEXT("LightError"), MB_OK | MB_ICONERROR);
		return;
	}
	lightBufferHeap.buffer->pointLightPos[_lightNum].x = _pos.X;
	lightBufferHeap.buffer->pointLightPos[_lightNum].y = _pos.Y;
	lightBufferHeap.buffer->pointLightPos[_lightNum].z = _pos.Z;

	lightBufferHeap.buffer->pointLightRange[_lightNum].x = _range;

	lightBufferHeap.buffer->pointLightColor[_lightNum].x = _color.X;
	lightBufferHeap.buffer->pointLightColor[_lightNum].y = _color.Y;
	lightBufferHeap.buffer->pointLightColor[_lightNum].z = _color.Z;

}
void Lights::SetSpotLight(int _lightNum, Vector3 _pos, float _range, Vector3 _target, float _anglerRange, Vector3 _color) {
	if (_lightNum >= frauEngine::POINT_LIGHT) {
		MessageBox(NULL, TEXT("ライトの数が多すぎます、定数が足りていません"), TEXT("LightError"), MB_OK | MB_ICONERROR);
		return;
	}
	lightBufferHeap.buffer->spotLightPos[_lightNum].x = _pos.X;
	lightBufferHeap.buffer->spotLightPos[_lightNum].y = _pos.Y;
	lightBufferHeap.buffer->spotLightPos[_lightNum].z = _pos.Z;

	lightBufferHeap.buffer->spotLightRange[_lightNum].x = _range;

	lightBufferHeap.buffer->spotLightTarget[_lightNum].x = _target.X;
	lightBufferHeap.buffer->spotLightTarget[_lightNum].y = _target.Y;
	lightBufferHeap.buffer->spotLightTarget[_lightNum].z = _target.Z;

	lightBufferHeap.buffer->spotLightAngularRange[_lightNum].x = _anglerRange;

	lightBufferHeap.buffer->spotLightColor[_lightNum].x = _color.X;
	lightBufferHeap.buffer->spotLightColor[_lightNum].y = _color.Y;
	lightBufferHeap.buffer->spotLightColor[_lightNum].z = _color.Z;

}