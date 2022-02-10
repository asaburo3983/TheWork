#include "ShaderResourceBuffer.h"
#include "FBX.h"
using namespace srBuffer;
using namespace lowApp;

ShaderResourceBufferHeap::ShaderResourceBufferHeap() {
	//TODO テクスチャロードのファンクションを作成する
		//拡張子ごとのテクスチャのロード形式を変更
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
	device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descripterHeap));//生成

	//通常テクスチャビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1
	srvDesc.Format = resource->GetDesc().Format;


	auto handle = descripterHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(resource, &srvDesc, handle);


}

void ShaderResourceBufferHeap::CreateShaderResourceBufferHeap(std::string _filePath) {

	// ファイル名を取得
	std::string file_path = _filePath;//フルパスを渡す

	// ファイル分解
	char buffer[256];
	ZeroMemory(buffer, sizeof(char) * 256);
	memcpy(buffer, file_path.c_str(), sizeof(char) * 256);

	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;//分割した文字列が入る
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);

	std::string root_path = _filePath;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::wstring wstr_file_name = cv.from_bytes(_filePath);//いろいろ処理したがめんどくさいのでフルパスを突っ込む

	// 文字化け対策
	DirectX::TexMetadata meta{};
	DirectX::ScratchImage img;

	std::wstring wext = GetExtension(wstr_file_name);
	std::string ext = GetStringFromWideString(wext);
	std::string filePath = _filePath;
	std::wstring filePathW = GetWideStringFromString(filePath);
	//テクスチャ読み込み処理

	auto result = loadTextureLambda[ext](filePathW, &meta, img);

	auto device = LowApplication::GetInstance()->GetDevice();


	//テクスチャからリソースを作成
	auto rawImg = img.GetImage(0, 0, 0);//生データ抽出

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;//特殊な設定なのでdefaultでもuploadでもなく
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//ライトバックで
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//転送がL0つまりCPU側から直で
	texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
	texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = meta.format;
	resDesc.Width = static_cast<UINT>(meta.width);//幅
	resDesc.Height = static_cast<UINT>(meta.height);//高さ
	resDesc.DepthOrArraySize = static_cast<UINT16>(meta.arraySize);
	resDesc.SampleDesc.Count = 1;//通常テクスチャなのでアンチェリしない
	resDesc.SampleDesc.Quality = 0;//
	resDesc.MipLevels = static_cast<UINT16>(meta.mipLevels);//ミップマップしないのでミップ数は１つ
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(meta.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;//レイアウトについては決定しない
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//とくにフラグなし

	ID3D12Resource* texbuff = nullptr;
	result = device->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,//特に指定なし
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	texbuff->WriteToSubresource(0,
		nullptr,//全領域へコピー
		rawImg->pixels,//元データアドレス
		static_cast<UINT>(rawImg->rowPitch),//1ラインサイズ
		static_cast<UINT>(rawImg->slicePitch)//全サイズ
	);


	//デスクリプタヒープを作成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descripterHeap));//生成

	//通常テクスチャビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1
	srvDesc.Format = texbuff->GetDesc().Format;


	auto handle = descripterHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(texbuff, &srvDesc, handle);
}