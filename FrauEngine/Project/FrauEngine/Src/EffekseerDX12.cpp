#include "EffekseerDX12.h"

using namespace frauEngine;

Effect::~Effect() {
	// マネージャーの破棄
	manager.Reset();
	// レンダラーの破棄
	renderer.Reset();
}

void Effect::Load(const char16_t* ufilepath) {
	auto lowApp = LowApplication::GetInstance();

	DXGI_FORMAT format;
	// エフェクトのレンダラーの作成
	format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderer = ::EffekseerRendererDX12::Create(lowApp->GetDevice(), lowApp->GetCommandQueue(), 3, &format, 1, DXGI_FORMAT_UNKNOWN, false, 8000);

	// メモリプールの作成
	sfMemoryPoolEfk = EffekseerRenderer::CreateSingleFrameMemoryPool(renderer->GetGraphicsDevice());

	// コマンドリストの作成
	commandListEfk = EffekseerRenderer::CreateCommandList(renderer->GetGraphicsDevice(), sfMemoryPoolEfk);

	// エフェクトのマネージャーの作成
	manager = ::Effekseer::Manager::Create(8000);

	// 描画モジュールの設定
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());
	manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// エフェクトの読込
	effect = Effekseer::Effect::Create(manager, ufilepath);

}
void Effect::Play(Vector3 _pos,Vector3 _rota,Vector3 _scale,int _endTime) {
	auto lowApp = LowApplication::GetInstance();

	// フレームの開始時に呼ぶ
	sfMemoryPoolEfk->NewFrame();

	frauEngine::Camera* cam = frauEngine::Camera::GetInstance();


	// 視点位置を確定
	auto g_position = ::Effekseer::Vector3D(cam->GetPos().X, cam->GetPos().Y, cam->GetPos().Z);
	// 投影行列を設定
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		cam->GetFov() / 180.0f * 3.14f, 
		static_cast<float>(lowApp->GetWindowSizeX()) / static_cast<float>(lowApp->GetWindowSizeY())
		, 0.1f, 1000.0f));
	// カメラ行列を設定
	renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(cam->GetTarget().X, cam->GetTarget().Y, cam->GetTarget().Z), ::Effekseer::Vector3D(cam->GetUp().X, cam->GetUp().Y, cam->GetUp().Z)));


	// コマンドリストを開始する。
	EffekseerRendererDX12::BeginCommandList(commandListEfk, lowApp->GetCommandList());
	renderer->SetCommandList(commandListEfk);

	if (time % _endTime == 0)
	{
		// エフェクトの再生
		handle = manager->Play(effect, _pos.X,_pos.Y,_pos.Z);
	}
	else if (time % _endTime == _endTime-1)
	{
		// エフェクトの停止
		manager->StopEffect(handle);
	}

	// エフェクトの移動
	manager->SetLocation(handle, ::Effekseer::Vector3D(_pos.X, _pos.Y, _pos.Z));
	manager->SetRotation(handle, _rota.X, _rota.Y, _rota.Z);
	manager->SetScale(handle, _scale.X, _scale.Y, _scale.Z);
	// マネージャーの更新
	manager->Update();
	// 時間を更新する
	renderer->SetTime(time / 60.0f);

	// エフェクトの描画開始処理を行う。
	renderer->BeginRendering();

	// エフェクトの描画を行う。
	manager->Draw();

	// エフェクトの描画終了処理を行う。
	renderer->EndRendering();

	// コマンドリストを終了する。
	renderer->SetCommandList(nullptr);
	EffekseerRendererDX12::EndCommandList(commandListEfk);

	time++;
}

void Effect::SetTime(int _time) {
	time = _time;
}

