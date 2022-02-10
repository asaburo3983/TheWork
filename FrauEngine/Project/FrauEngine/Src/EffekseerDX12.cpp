#include "EffekseerDX12.h"

using namespace frauEngine;

Effect::~Effect() {
	// �}�l�[�W���[�̔j��
	manager.Reset();
	// �����_���[�̔j��
	renderer.Reset();
}

void Effect::Load(const char16_t* ufilepath) {
	auto lowApp = LowApplication::GetInstance();

	DXGI_FORMAT format;
	// �G�t�F�N�g�̃����_���[�̍쐬
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderer = ::EffekseerRendererDX12::Create(lowApp->GetDevice(), lowApp->GetCommandQueue(), 3, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	// �������v�[���̍쐬
	sfMemoryPoolEfk = EffekseerRenderer::CreateSingleFrameMemoryPool(renderer->GetGraphicsDevice());

	// �R�}���h���X�g�̍쐬
	commandListEfk = EffekseerRenderer::CreateCommandList(renderer->GetGraphicsDevice(), sfMemoryPoolEfk);

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	manager = ::Effekseer::Manager::Create(8000);

	// �`�惂�W���[���̐ݒ�
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());
	manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// �G�t�F�N�g�̓Ǎ�
	effect = Effekseer::Effect::Create(manager, ufilepath);

}
void Effect::Play(Vector3 _pos,Vector3 _rota,Vector3 _scale,int _endTime) {
	auto lowApp = LowApplication::GetInstance();

	// �t���[���̊J�n���ɌĂ�
	sfMemoryPoolEfk->NewFrame();

	frauEngine::Camera* cam = frauEngine::Camera::GetInstance();


	// ���_�ʒu���m��
	auto g_position = ::Effekseer::Vector3D(cam->GetPos().X, cam->GetPos().Y, cam->GetPos().Z);
	// ���e�s���ݒ�
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		cam->GetFov() / 180.0f * 3.14f, 
		static_cast<float>(lowApp->GetWindowSizeX()) / static_cast<float>(lowApp->GetWindowSizeY())
		, 0.1f, 1000.0f));
	// �J�����s���ݒ�
	renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(cam->GetTarget().X, cam->GetTarget().Y, cam->GetTarget().Z), ::Effekseer::Vector3D(cam->GetUp().X, cam->GetUp().Y, cam->GetUp().Z)));


	// �R�}���h���X�g���J�n����B
	EffekseerRendererDX12::BeginCommandList(commandListEfk, lowApp->GetCommandList());
	renderer->SetCommandList(commandListEfk);

	if (time % _endTime == 0)
	{
		// �G�t�F�N�g�̍Đ�
		handle = manager->Play(effect, _pos.X,_pos.Y,_pos.Z);
	}
	else if (time % _endTime == _endTime-1)
	{
		// �G�t�F�N�g�̒�~
		manager->StopEffect(handle);
	}

	// �G�t�F�N�g�̈ړ�
	manager->SetLocation(handle, ::Effekseer::Vector3D(_pos.X, _pos.Y, _pos.Z));
	manager->SetRotation(handle, _rota.X, _rota.Y, _rota.Z);
	manager->SetScale(handle, _scale.X, _scale.Y, _scale.Z);
	// �}�l�[�W���[�̍X�V
	manager->Update();
	// ���Ԃ��X�V����
	renderer->SetTime(time / 60.0f);

	// �G�t�F�N�g�̕`��J�n�������s���B
	renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	renderer->EndRendering();

	// �R�}���h���X�g���I������B
	renderer->SetCommandList(nullptr);
	EffekseerRendererDX12::EndCommandList(commandListEfk);

	time++;
}

void Effect::SetTime(int _time) {
	time = _time;
}

