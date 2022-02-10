
#include "Player.h"
#include "PlanterSystem.h"
#include "SoundManager.h"

Player::Player() {
	KEY = KeyInput::GetInstance();
}
void Player::Initialize(float _moveSpeed, float _rotaSpeed) {
	auto resource = Resource::GetInstance();

	//プレイヤーモデルの用意
	model.Initialize(resource->Model("Frau.fbx"), resource->Shader("VertexShader_Anim.hlsl"), resource->Shader("PixelShader_Toon.hlsl"), nullptr, nullptr, nullptr, D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT);
	model.InitializeSub(ModelType::SHADOW, resource->Shader("VertexShader_AnimShadow.hlsl"), resource->Shader("PixelShader_Shadow.hlsl"));
	LoadMaterialLinker("Data/Model/Frau/MatLink/Frau.matlink", "Data/Model/Frau/MatLink/Material/", "Data/Model/Frau/Tex/", &model);

	auto vertexShaderBillBoard = resource->Shader("VertexShader_BillBoard.hlsl");
	auto pixelShaderBillBoard = resource->Shader("PixelShader_BillBoard.hlsl");

	//ビルボード用のモデルを用意
	ideaFrame.SetBillboard(true);
	ideaFrame.Initialize(
		resource->Model("Idea_Frame.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);

	for (int i = 0; i < (int)IdeaNum::MAX; i++) {
		ideaStr[i].SetBillboard(true);
	}
	ideaStr[(int)IdeaNum::BUY].Initialize(
		resource->Model("Idea_Buy.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);
	ideaStr[(int)IdeaNum::EXIT].Initialize(
		resource->Model("Idea_Exit.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);
	ideaStr[(int)IdeaNum::OPEN_SHOP].Initialize(
		resource->Model("Idea_OpenShop.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);
	ideaStr[(int)IdeaNum::PLANTER].Initialize(
		resource->Model("Idea_Planter.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);
	ideaStr[(int)IdeaNum::JOIN_SHOP].Initialize(
		resource->Model("Idea_JoinShop.fbx"),
		vertexShaderBillBoard,
		pixelShaderBillBoard
	);

	model.SetScale(Vector3(0.03f, 0.03f, 0.03f));
	model.SetAngle(Vector3(0, 180, 0));
	SetStartPos();

	moveSpeed = _moveSpeed;
	rotaSpeed = _rotaSpeed;

}
void Player::MoveStage() {
	SetStartPos();
}
void Player::SetStartPos() {
	Stage* stage = Stage::GetInstance();
	switch (stage->GetStageNum()) {
	case (int)StageNum::FLOWER_SHOP:
		model.SetPos(Vector3(-2.55, -3.9f, -2));
		model.SetAngle(Vector3(0, 0, 0));
		break;
	case (int)StageNum::HANDY_SHOP:
		model.SetPos(Vector3(0, -3.9f, -2));
		model.SetAngle(Vector3(0, 0, 0));
		break;
	case (int)StageNum::MAGIC_SHOP:
		model.SetPos(Vector3(1.85, -3.9f, -1.2));
		model.SetAngle(Vector3(0, 0, 0));
		break;
	case (int)StageNum::MAP:
		//出口ごとに違う場所へ
		switch (stage->GetStageNumOld()) {
		case (int)StageNum::FLOWER_SHOP:
			model.SetPos(Vector3(4.75, -3.9f, -8.3));
			break;
		case (int)StageNum::HANDY_SHOP:
			model.SetPos(Vector3(8.1, 1.45, 2.45));
			break;
		case (int)StageNum::MAGIC_SHOP:
			model.SetPos(Vector3(-13.0, 1.45, 2.45));
			break;
		}
		
		
		model.SetAngle(Vector3(0, 180, 0));
		break;
	}
}

void Player::Update() {
	if (isAnimation) {
		AnimationControl();
	}
	if (isMove &&
		model.GetAnimeNum() != 3 &&
		model.GetAnimeNum() != 4 ) {
		Move();
	}
	else if(model.GetAnimeNum() == 3 || model.GetAnimeNum() == 4){
		float target = -1, targetX = -1, targetY = 270;
		SetVector(target, targetX, targetY);
	}
	if (isCollision) {
		Collision();
	}
}


void Player::AnimationControl() {
	auto sound = SoundManager::GetInstance();
	auto planter = PlanterSystem::GetInstance();
	auto novelSystem = NovelSystem::GetInstance();

	//ノベル中はStay状態で固定
	if (novelSystem->GetEnable()) {
		model.SetAnimeNum(1);
		model.SetAnimeSpeed(0.3f);
		walkSoundCount = 0;
		return;
	}

	if (planter->GetEnable()) {
		if (sitAnimCount < sitAnimCountMax) {
			model.SetAnimeNum(3);
			model.SetAnimeSpeed(0.3f);
			sitAnimCount++;
			
		}
		else {
			model.SetAnimeNum(4);
			model.SetAnimeSpeed(0.3f);
		}
	}
	else {
		if (sitAnimCount>0) {			
			if (model.GetAnimeNum() == 4) {
				model.SetAnimeNum(3);
				model.SetAnimeSpeed(-0.3f);
				model.SetAnimeTime(sitAnimCountMax * 0.3);
			}
			sitAnimCount--;
		}
		else {
			//歩きモーション
			if (KEY->key[DIK_A] > 0 ||
				KEY->key[DIK_D] > 0 ||
				KEY->key[DIK_W] > 0 ||
				KEY->key[DIK_S] > 0) {			
				model.SetAnimeNum(2);
				model.SetAnimeSpeed(0.5f);
				
				walkSoundCount++;
				if (walkSoundCount > walkSoundCountMax) {
					walkSoundCount = 0;
					sound->GetSE(SoundList_SE::WALK)->Play();
				}
			}
			//待機モーション
			else {			
				model.SetAnimeNum(1);
				model.SetAnimeSpeed(0.3f);
				walkSoundCount = 0;
			}
		}
	}
}
void Player::SetVector(float _target, float _targetX, float _targetY) {
	Vector3 angle = model.GetAngle();

	//向きの計算
	if (_targetX != -1 && _targetY != -1) {
		if (_targetX == 360 || _targetX == 0) {
			if (_targetY > 180) {
				_targetX = 360;
			}
			else {
				_targetX = 0;
			}
		}
		_target = (_targetX + _targetY) / 2;
	}
	else {
		if (_targetX >= 0) {
			_target = _targetX;
		}
		else {
			_target = _targetY;
		}
	}
	//回転処理
	if (_target != -1) {
		if (_target > angle.Y) {
			float tmp = _target - angle.Y;
			if (tmp <= 180) {
				angle.Y += rotaSpeed;
			}
			else {
				angle.Y -= rotaSpeed;
			}
		}
		else if (_target < angle.Y) {
			int tmp = angle.Y - _target;
			if (tmp < 180) {
				angle.Y -= rotaSpeed;
			}
			else {
				angle.Y += rotaSpeed;
			}
		}
		if (angle.Y < 0) {
			angle.Y = 360;
		}
		if (angle.Y > 360) {
			angle.Y = 0;
		}
	}

	model.SetAngle(angle);
}
void Player::Move() {
	posOld = model.GetPos();
	Vector3 pos = model.GetPos();
	Vector3 angle = model.GetAngle();

	float target = -1, targetX = -1, targetY = -1;

	if (KEY->key[DIK_A] > 0) {
		pos.X -= moveSpeed;
		targetY = 270;
	}
	else if (KEY->key[DIK_D] > 0) {
		pos.X += moveSpeed;
		targetY = 90;
	}

	if (KEY->key[DIK_W] > 0) {
		pos.Z += moveSpeed;
		targetX = 0;
		if (angle.Y > 180) {
			targetX = 360;
		}
	}
	else if (KEY->key[DIK_S] > 0.0f) {
		pos.Z -= moveSpeed;
		targetX = 180;
	}

	//向きたい方向にモデル角度を変更
	SetVector(target, targetX, targetY);

	model.SetPos(pos);

}
void Player::Collision() {
	Stage* stage = Stage::GetInstance();
	//当たり判定処理
	for (int i = 0; i < stage->boxCollider.size(); i++) {
		Vector3 tmp;
		bool moveBack[2] = { false,false };

		if (stage->boxCollider[i].HitX(GetPos().X) &&
			stage->boxCollider[i].HitY(GetPosOld().Z)) {
			moveBack[0] = true;
		}
		if (stage->boxCollider[i].HitX(GetPosOld().X) &&
			stage->boxCollider[i].HitY(GetPos().Z)) {
			moveBack[1] = true;
		}
		if (moveBack[0] == true) {
			MoveBackX();
		}
		if (moveBack[1] == true) {
			MoveBackZ();
		}
	}
	for (int i = 0; i < stage->circleCollider.size(); i++) {
		Vector3 tmp;
		bool moveBack[2] = { false,false };
		if (stage->circleCollider[i].Hit(Vector2(GetPos().X, GetPosOld().Z), 1)) {
			moveBack[0] = true;
		}
		if (stage->circleCollider[i].Hit(Vector2(GetPosOld().X, GetPos().Z), 1)) {
			moveBack[1] = true;
		}
		if (moveBack[0] == true) {
			MoveBackX();
		}
		if (moveBack[1] == true) {
			MoveBackZ();
		}
	}

	//当たり判定処理スロープ
	for (int i = 0; i < stage->slopeCollider.size(); i++) {
		Vector2 pos;
		pos.X = GetPos().X;
		pos.Y = GetPos().Z;

		//スロープを上下する処理
		if (stage->slopeCollider[i].Hit(pos)) {
			Vector3 tmp = GetPos();
			tmp.Y = stage->slopeCollider[i].HitProcess(pos);		
			model.SetPos(tmp);
		}
	}
	//当たり判定処理　イベント
	for (int i = 0; i < stage->boxColliderEvent.size(); i++) {
		if (stage->boxColliderEvent[i].Hit(Vector2(GetPos().X, GetPos().Z))) {
			eventNum = stage->boxColliderEvent[i].GetEventNum();

			break;
		}
		eventNum = -1;
	}

}


void Player::DrawShadow() {

	model.SetAllAnimeState(true, model.GetAnimeNum(),  model.GetAnimeSpeed());
	model.SetAll(model.GetPos(), model.GetAngle(), model.GetScale());
	model.Draw(ModelType::SHADOW);
}
void Player::Draw() {

	model.ModelObject::Draw();


}
void Player::DrawBillBoard() {
	bool handyShop = HandyShop::GetInstance()->GetEnable();
	bool magicShop = MagicShop::GetInstance()->GetEnable();
	bool novel = NovelSystem::GetInstance()->GetEnable();
	bool planter = PlanterSystem::GetInstance()->GetEnable();
	if (eventNum != -1 && 
		handyShop == false && magicShop == false&&
		novel == false &&
		planter == false
		) {
		int eventNumTmp = eventNum;

		Vector3 pos = GetPos();
		pos.Y += 5.0f;
		pos.X += 2.0;
		ideaFrame.SetAll(pos, Vector3(0, 0, 0), Vector3(1.5, 1, 1));

		ideaFrame.Draw();

		//イベント上では違う処理だが表示するものは一緒
		switch (eventNum) {
		case (int)EventNum::BUY_HANDYSHOP:
		case (int)EventNum::BUY_MAGICSHOP:
			eventNumTmp = (int)IdeaNum::BUY;
			break;
		case (int)EventNum::JOIN_FLOWERSHOP:
		case (int)EventNum::JOIN_HANDYSHOP:
		case (int)EventNum::JOIN_MAGICSHOP:
			eventNumTmp = (int)IdeaNum::JOIN_SHOP;
			break;
		}

		ideaStr[eventNumTmp].SetAll(pos, Vector3(0, 180.0f, 0), Vector3(1.0, 0.77, 1));
		ideaStr[eventNumTmp].Draw();
	}
}
