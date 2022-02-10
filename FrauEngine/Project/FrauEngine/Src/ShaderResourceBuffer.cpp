#include "ShaderResourceBuffer.h"

using namespace frauEngine;
using namespace DirectX;
using namespace std;

ShaderResourceBufferHeap::ShaderResourceBufferHeap() {
		//�g���q���Ƃ̃e�N�X�`���̃��[�h�`����ύX
	loadTextureLambda["sph"] = loadTextureLambda["spa"] = loadTextureLambda["bmp"] = loadTextureLambda["png"] = loadTextureLambda["jpg"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, meta, img);
	};
	loadTextureLambda["tga"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromTGAFile(path.c_str(), meta, img);
	};
	loadTextureLambda["dds"] = [](const wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, meta, img);
	};
}

void ShaderResourceBufferHeap::CreateShaderResourceBufferHeap(ID3D12Resource* resource) {

	auto device = LowApplication::GetInstance()->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descripterHeap));//����

	//�ʏ�e�N�X�`���r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;//�~�b�v�}�b�v�͎g�p���Ȃ��̂�1
	srvDesc.Format = resource->GetDesc().Format;


	auto handle = descripterHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(resource, &srvDesc, handle);


}

void ShaderResourceBufferHeap::CreateShaderResourceBufferHeap(std::string _filePath) {

	// �t�@�C�������擾
	std::string file_path = _filePath;//�t���p�X��n��

	// �t�@�C������
	char buffer[256];
	sprintf_s(buffer, file_path.c_str());
	// �L������
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;//�������������񂪓���
	std::string replace_file_name = buffer;
	// �u/�v�ŕ���
	Split('/', buffer, split_list);

	std::string root_path = _filePath;

	// ���������΍�
	DirectX::TexMetadata meta{};
	DirectX::ScratchImage img;

	//std::wstring wext = GetExtension(wstr_file_name);
	std::string ext = GetExtension(split_list[split_list.size() - 1]);
	std::string filePath = _filePath;
	std::wstring filePathW = GetWideStringFromString(filePath);
	//�e�N�X�`���ǂݍ��ݏ���

	auto result = loadTextureLambda[ext](filePathW, &meta, img);

	auto device = LowApplication::GetInstance()->GetDevice();


	//�e�N�X�`�����烊�\�[�X���쐬
	auto rawImg = img.GetImage(0, 0, 0);//���f�[�^���o

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//����Ȑݒ�Ȃ̂�default�ł�upload�ł��Ȃ�
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//���C�g�o�b�N��
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//�]����L0�܂�CPU�����璼��
	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = meta.format;
	resDesc.Width = static_cast<UINT>(meta.width);//��
	resDesc.Height = static_cast<UINT>(meta.height);//����
	resDesc.DepthOrArraySize = static_cast<UINT16>(meta.arraySize);
	resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<UINT16>(meta.mipLevels);//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(meta.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�

	ID3D12Resource* texbuff = nullptr;
	result = device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	texbuff->WriteToSubresource(0,
		nullptr,//�S�̈�փR�s�[
		rawImg->pixels,//���f�[�^�A�h���X
		static_cast<UINT>(rawImg->rowPitch),//1���C���T�C�Y
		static_cast<UINT>(rawImg->slicePitch)//�S�T�C�Y
	);


	//�f�X�N���v�^�q�[�v���쐬
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descripterHeap));//����

	//�ʏ�e�N�X�`���r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;//�~�b�v�}�b�v�͎g�p���Ȃ��̂�1
	srvDesc.Format = texbuff->GetDesc().Format;


	auto handle = descripterHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(texbuff, &srvDesc, handle);
}