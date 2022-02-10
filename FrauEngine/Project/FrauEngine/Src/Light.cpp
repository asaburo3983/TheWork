#include "Light.h"

using namespace frauEngine;

Lights::Lights() {
	//�[�x�������
	
	auto device = LowApplication::GetInstance()->GetDevice();

	//�萔�o�b�t�@�̍쐬
	lightBufferHeap.Create();

	//�[�x�o�b�t�@�̎d�l
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;//2�����̃e�N�X�`���f�[�^�Ƃ���
	depthResDesc.Width = shadowTexSize;//���ƍ����̓����_�[�^�[�Q�b�g�Ɠ���
	depthResDesc.Height =shadowTexSize;//��ɓ���
	depthResDesc.DepthOrArraySize = 1;//�e�N�X�`���z��ł��Ȃ���3D�e�N�X�`���ł��Ȃ�
	depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;//�[�x�l�������ݗp�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;//�T���v����1�s�N�Z��������1��
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//���̃o�b�t�@�͐[�x�X�e���V���Ƃ��Ďg�p���܂�
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.MipLevels = 1;
	//�f�v�X�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//DEFAULT��������UNKNOWN�ł悵
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	//���̃N���A�o�����[���d�v�ȈӖ�������
	D3D12_CLEAR_VALUE _depthClearValue = {};
	_depthClearValue.DepthStencil.Depth = 1.0f;//�[���P(�ő�l)�ŃN���A
	_depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//32bit�[�x�l�Ƃ��ăN���A

	ID3D12Resource* depthBuffer = nullptr;
	auto result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�f�v�X�������݂Ɏg�p
		&_depthClearValue,
		IID_PPV_ARGS(&depthBuffer));


	//�[�x�̂��߂̃f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};//�[�x�Ɏg����Ƃ��������킩��΂���
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[�Ƃ��Ďg��
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&depthHeap));



	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�f�v�X�l��32bit�g�p
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;//�t���O�͓��ɂȂ�

	auto handle = depthHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(depthBuffer, &dsvDesc, handle);//�J�������猩���[�x


	//�[�x�̃V�F�[�_�[���\�[�X�p�f�X�N���v�V�����q�[�v�̍쐬
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
	//�J�������猩���f�v�X�e�N�X�`��
	device->CreateShaderResourceView(depthBuffer, &resDesc, handle);

	//�����̏����l�����Ă���
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
		0.1f,//�߂���
		1000.0f//������
	);
}

void Lights::SetPointLight(int _lightNum, Vector3 _pos, float _range, Vector3 _color) {
	if (_lightNum >= frauEngine::POINT_LIGHT) {
		MessageBox(NULL, TEXT("���C�g�̐����������܂��A�萔������Ă��܂���"), TEXT("LightError"), MB_OK | MB_ICONERROR);
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
		MessageBox(NULL, TEXT("���C�g�̐����������܂��A�萔������Ă��܂���"), TEXT("LightError"), MB_OK | MB_ICONERROR);
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