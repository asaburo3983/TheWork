#include "DXTK.h"

using namespace frauEngine;
using namespace DirectX;


void Font::DrawString(std::string _str , Vector2 _pos , float _scale, Color _color , bool _center, float _angle ) {
	auto manager = ManagerDXTK::GetInstance();

	frauEngine::ManagerDXTK* def = frauEngine::ManagerDXTK::GetInstance();

	ID3D12GraphicsCommandList* commandList = frauEngine::LowApplication::GetInstance()->GetCommandList();
	ID3D12DescriptorHeap* heaps[] = { manager->resourceDescriptors->Heap(), def->commonStates->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	manager->sprites->Begin(frauEngine::LowApplication::GetInstance()->GetCommandList());
	XMVECTORF32 color = { {{_color.R,_color.G,_color.B,_color.A}} };


	WCHAR* str = GetWCharFromChar(_str.c_str());
	int length = _str.length() / 2;
	int sizeX2 = length * pixelSize / 2 * _scale * allSize;
	int sizeY2 =          pixelSize / 2	* _scale * allSize;

	font->DrawString(manager->sprites.get(),
		str,
		XMFLOAT2(_pos.X, _pos.Y),
		color,
		_angle,
		DirectX::SimpleMath::Vector2(_center*sizeX2, _center*sizeY2),
		_scale * allSize);

	manager->sprites->End();

}void Font::DrawString(std::string _str , Vector2 _pos , float _scale, Color _color , Vector2 _anchor, float _angle ) {
	auto manager = ManagerDXTK::GetInstance();

	frauEngine::ManagerDXTK* def = frauEngine::ManagerDXTK::GetInstance();

	ID3D12GraphicsCommandList* commandList = frauEngine::LowApplication::GetInstance()->GetCommandList();
	ID3D12DescriptorHeap* heaps[] = { manager->resourceDescriptors->Heap(), def->commonStates->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	manager->sprites->Begin(frauEngine::LowApplication::GetInstance()->GetCommandList());
	XMVECTORF32 color = { {{_color.R,_color.G,_color.B,_color.A}} };


	WCHAR* str = GetWCharFromChar(_str.c_str());
	int length = _str.length() / 2;
	int sizeX = length * pixelSize  * _scale * allSize;
	int sizeY =          pixelSize  * _scale * allSize;

	font->DrawString(manager->sprites.get(),
		str,
		XMFLOAT2(_pos.X, _pos.Y),
		color,
		_angle,
		DirectX::SimpleMath::Vector2(_anchor.X*sizeX, _anchor.Y*sizeY),
		_scale * allSize);

	manager->sprites->End();

}

void ManagerDXTK::Init() {
	auto device = LowApplication::GetInstance()->GetDevice();
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	graphicsMemory = std::make_unique<GraphicsMemory>(device);
	commonStates = std::make_unique<CommonStates>(device);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pl = LowApplication::GetInstance()->GetPipelineStateDesc(frauEngine::PipelineType::DEFAULT);
	RenderTargetState rt;
	rt.numRenderTargets = pl.NumRenderTargets;
	rt.dsvFormat = pl.DSVFormat;
	rt.nodeMask = pl.NodeMask;
	rt.rtvFormats[0] = pl.RTVFormats[0];
	rt.sampleDesc = pl.SampleDesc;
	rt.sampleMask = pl.SampleMask;

	SpriteBatchPipelineStateDescription pd(rt);

	pd.blendDesc = pl.BlendState;
	pd.depthStencilDesc = pl.DepthStencilState;
	pd.depthStencilDesc.DepthEnable = false;//深度バッファを無効にする

	pd.rasterizerDesc = pl.RasterizerState;
	pd.renderTargetState = rt;
	pd.customRootSignature = pl.pRootSignature;

	sprites = std::make_unique<SpriteBatch>(device, resourceUpload, pd);

	//フォント処理を書き込む
	auto fonts = Fonts::GetInstance();
	setlocale(LC_ALL, "japanese");
	resourceDescriptors = std::make_unique<DescriptorHeap>(device, 256);
	for (int i = 0; i < (int)FontList::MAX; i++) {
		
		
		fonts->font[i].font = std::make_unique<SpriteFont>(device, resourceUpload, fonts->fontPass[i],
			resourceDescriptors->GetCpuHandle(i),
			resourceDescriptors->GetGpuHandle(i));
	}
	sprites->SetViewport(LowApplication::GetInstance()->GetViewPort(frauEngine::PipelineType::DEFAULT));

	auto uploadResourcesFinished = resourceUpload.End(LowApplication::GetInstance()->GetCommandQueue());

	uploadResourcesFinished.wait();
}
void ManagerDXTK::UnInit() {
	sprites.reset();
	commonStates.reset();
	graphicsMemory.reset();
}
void ManagerDXTK::DrawFinish() {
	//２D描画の１ループで１回呼ぶ、Present後
	graphicsMemory->Commit(LowApplication::GetInstance()->GetCommandQueue());
}
void ManagerDXTK::SetImageEX(float ex) {
	imageExtend = ex;
}
float ManagerDXTK::GetImageEX() {
	return imageExtend;
}

Sound_MP3::Sound_MP3(const wchar_t* filePath) {
	Load(filePath);
}
void Sound_MP3::Load(const wchar_t* filePath) {
	MFPCreateMediaPlayer(filePath, FALSE, 0, NULL, NULL, &mediaPlayer);
}
void Sound_MP3::Load(const char* filePath) {
	size_t tmpsize = strlen(filePath);//char型をWCHARに変換
	int  tmpsizei = tmpsize + 1;
	WCHAR* tmpwbuf = new WCHAR[tmpsizei];
	mbstowcs_s(0, tmpwbuf, tmpsizei, filePath, _TRUNCATE);

	MFPCreateMediaPlayer(tmpwbuf, FALSE, MFP_OPTION_FREE_THREADED_CALLBACK, NULL, NULL, &mediaPlayer);
	delete tmpwbuf;
}
void Sound_MP3::Play(bool loop) {

	PROPVARIANT vPositionValue;
	PropVariantInit(&vPositionValue);
	vPositionValue.vt = VT_I8;
	vPositionValue.hVal.QuadPart = 0; // 1秒 (10000000[100nsec]) の位置から再生する;

	if (!loop) {
		mediaPlayer->SetPosition(
			MFP_POSITIONTYPE_100NS,
			&vPositionValue
		);
	}
	mediaPlayer->GetState(&mediaPlayerState);

	if (mediaPlayerState == MFP_MEDIAPLAYER_STATE_STOPPED || mediaPlayerState == MFP_MEDIAPLAYER_STATE_EMPTY)
	{	
		mediaPlayer->Play();
	}
}
void Sound_MP3::Stop() {
	mediaPlayer->GetState(&mediaPlayerState);
	if (mediaPlayerState != MFP_MEDIAPLAYER_STATE_STOPPED) {
		mediaPlayer->Stop();
	}
}
void Sound_MP3::Volume(float vol) {
	auto result =mediaPlayer->SetVolume(vol);
	float vol2;
	mediaPlayer->GetVolume(&vol2);
}

void Image2D::SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource) {
	//低レベルで生成したテクスチャを割り当てる
	auto device = LowApplication::GetInstance()->GetDevice();

	resourceDescriptors = std::make_unique<DescriptorHeap>(device, 256);

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	texture = resource;

	resourceUpload.End(LowApplication::GetInstance()->GetCommandQueue());
}
void Image2D::Load(const char* filepath) {

    auto device=LowApplication::GetInstance()->GetDevice();

	resourceDescriptors = std::make_unique<DescriptorHeap>(device, 256);

    ResourceUploadBatch resourceUpload(device);
    resourceUpload.Begin();

	CreateWICTextureFromFile(device, resourceUpload, GetWCharFromChar(filepath), texture.ReleaseAndGetAddressOf());

    CreateShaderResourceView(device, texture.Get(), resourceDescriptors->GetCpuHandle(0));//とりあえずハンドル番号は０番

	resourceUpload.End(LowApplication::GetInstance()->GetCommandQueue());

	//サイズデータを保持しておく
	width = GetTextureSize(texture.Get()).x;
	height= GetTextureSize(texture.Get()).y;
}
void Image2D::Draw(int _x,int _y,float _scaleX,float _scaleY,float _angle,float _alpha,bool _turnLR,bool _turnUD,bool _center) {
	frauEngine::ManagerDXTK* def = frauEngine::ManagerDXTK::GetInstance();

	ID3D12GraphicsCommandList* commandList = frauEngine::LowApplication::GetInstance()->GetCommandList();
	ID3D12DescriptorHeap* heaps[] = { resourceDescriptors->Heap(), def->commonStates->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	
	float EX= def->GetImageEX();
	int x = _x * EX;
	int y = _y * EX;
	float scaleX = _scaleX * EX;
	float scaleY = _scaleY * EX;

	//描画範囲（固定）
	RECT drawArea;
	drawArea.left = 0;
	drawArea.top = 0;
	drawArea.right = width;
	drawArea.bottom = height;
	const RECT* rect = &drawArea;

	//反転
	SpriteEffects effects;
	if (!_turnLR && !_turnUD) {
		effects = DirectX::SpriteEffects_None;
	}
	else if (_turnLR) {
		effects = DirectX::SpriteEffects_FlipHorizontally;
	}
	else if (_turnUD) {
		effects = DirectX::SpriteEffects_FlipVertically;
	}
	else {
		effects = DirectX::SpriteEffects_FlipBoth;
	}

    def->sprites->Begin(frauEngine::LowApplication::GetInstance()->GetCommandList());

	def->sprites->Draw(resourceDescriptors->GetGpuHandle(0)
		, GetTextureSize(texture.Get()),
		XMFLOAT2(x, y),
		rect,
		DirectX::Colors::White * _alpha * def->GetAllImageMultiAlpha(),
		_angle / 180.0 * 3.1415,
		DirectX::SimpleMath::Vector2(width / 2, height / 2) * _center,
		XMFLOAT2(scaleX,scaleY),
		effects
	);
    def->sprites->End();

}

UINT Image2D::GetWidth() {
	return width;
}
UINT Image2D::GetHeight() {
	return height;
}