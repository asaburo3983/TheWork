#include "DXTK.h"

using namespace dxtk;
using namespace lowApp;
using namespace DirectX;

void ManagerDXTK::Init() {
	auto device = LowApplication::GetInstance()->GetDevice();
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	graphicsMemory = std::make_unique<GraphicsMemory>(device);
	commonStates = std::make_unique<CommonStates>(device);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pl = LowApplication::GetInstance()->GetPipelineStateDesc(lowApp::PipelineType::DEFAULT);
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
	pd.depthStencilDesc.DepthEnable = false;//�[�x�o�b�t�@�𖳌��ɂ���

	pd.rasterizerDesc = pl.RasterizerState;
	pd.renderTargetState = rt;
	pd.customRootSignature = pl.pRootSignature;

	sprites = std::make_unique<SpriteBatch>(device, resourceUpload, pd);
	sprites->SetViewport(LowApplication::GetInstance()->GetViewPort(lowApp::PipelineType::DEFAULT));

	auto uploadResourcesFinished = resourceUpload.End(LowApplication::GetInstance()->GetCommandQueue());

	uploadResourcesFinished.wait();
}
void ManagerDXTK::UnInit() {
	sprites.reset();
	commonStates.reset();
	graphicsMemory.reset();
}
void ManagerDXTK::DrawFinish() {
	//�QD�`��̂P���[�v�łP��ĂԁAPresent��
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
	size_t tmpsize = strlen(filePath);//char�^��WCHAR�ɕϊ�
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
	vPositionValue.hVal.QuadPart = 0; // 1�b (10000000[100nsec]) �̈ʒu����Đ�����;

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
	//�჌�x���Ő��������e�N�X�`�������蓖�Ă�
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

    CreateShaderResourceView(device, texture.Get(), resourceDescriptors->GetCpuHandle(0));//�Ƃ肠�����n���h���ԍ��͂O��

	resourceUpload.End(LowApplication::GetInstance()->GetCommandQueue());

	//�T�C�Y�f�[�^��ێ����Ă���
	width = GetTextureSize(texture.Get()).x;
	height= GetTextureSize(texture.Get()).y;
}
void Image2D::Draw(int _x,int _y,float _scaleX,float _scaleY,float _angle,float _alpha,bool _turnLR,bool _turnUD,bool _center) {
	dxtk::ManagerDXTK* def = dxtk::ManagerDXTK::GetInstance();

	ID3D12GraphicsCommandList* commandList = lowApp::LowApplication::GetInstance()->GetCommandList();
	ID3D12DescriptorHeap* heaps[] = { resourceDescriptors->Heap(), def->commonStates->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	
	float EX= def->GetImageEX();
	int x = _x * EX;
	int y = _y * EX;
	float scaleX = _scaleX * EX;
	float scaleY = _scaleY * EX;

	//�`��͈́i�Œ�j
	RECT drawArea;
	drawArea.left = 0;
	drawArea.top = 0;
	drawArea.right = width;
	drawArea.bottom = height;
	const RECT* rect = &drawArea;

	//���]
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

    def->sprites->Begin(lowApp::LowApplication::GetInstance()->GetCommandList());

	def->sprites->Draw(resourceDescriptors->GetGpuHandle(0)
		, GetTextureSize(texture.Get()),
		XMFLOAT2(x, y),
		rect,
		DirectX::Colors::White * _alpha,
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