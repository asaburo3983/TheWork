#include "CameraWork.h"


void CameraWork::Initialize() {

	Vector3 posOld;
	Vector3 targetOld;


	camera.SetPos(Vector3(0, 5, -13));
	camera.SetTarget(Vector3(0, 0, 0));
	camera.SetUp(Vector3(0, 1, 0));
	camera.SetFov(45);

	pos.resize(10);
	target.resize(10);

	//通常カメラ
	pos[0] = Vector3(0, 5, -13);
	target[0] = Vector3(0, 0, 0);
	//導入のカメラワーク
	pos[1] = Vector3(2, 1, -2);
	target[1] = Vector3(7, 0, 10);
	//購入＿ハンディのカメラワーク
	pos[2] = Vector3(-2, 3, -6);
	target[2] = Vector3(0, 0, 2);

	//購入＿マジックのカメラワーク
	pos[3] = Vector3(0, 2, -5);
	target[3] = Vector3(-7, 0, 0);


}
void CameraWork::Move() {
	if (lerpCount < 1.0f) {
		lerpCount += lerpSpeed;
	}
	Vector3 tmp = Vector3(
		lerp(pos[moveNumOld].X, pos[moveNum].X, lerpCount),
		lerp(pos[moveNumOld].Y, pos[moveNum].Y, lerpCount),
		lerp(pos[moveNumOld].Z, pos[moveNum].Z, lerpCount));
	camera.SetPos(tmp);

	tmp = Vector3(
		lerp(target[moveNumOld].X, target[moveNum].X, lerpCount),
		lerp(target[moveNumOld].Y, target[moveNum].Y, lerpCount),
		lerp(target[moveNumOld].Z, target[moveNum].Z, lerpCount));

	camera.SetTarget(tmp);
}
void CameraWork::PlayerTarget() {
	auto player=Player::GetInstance();

	auto pos = player->GetPos();
	pos.Y += 8.0f;
	pos.Z -= 10.0f;
	auto target = player->GetPos();
	target.Y += 4.0f;

	camera.SetPos(pos);
	camera.SetTarget(target);
}
void CameraWork::SetPhotoMode(bool _enable) {
	auto player = Player::GetInstance();

	photoMode = _enable; 
	//フォトモード有効にしたときだけカメラの位置を変更
	if (_enable) {
		Vector3 targetPos = player->GetPos();
		targetPos.Y += 3.5f;
		Vector3 cameraPos = player->GetPos();
		cameraPos.Y += 3.5f;
		cameraPos.Z -= 5;
		camera.SetPos(cameraPos);
		camera.SetTarget(targetPos);
	}
}
void CameraWork::PhotoMode() {
	auto player = Player::GetInstance();
	auto key = KeyInput::GetInstance();

	float cameraSpeed = 0.05f;
	Vector2 cameraMaxDist = { 3.0f ,3.0f };
	Vector3 cameraPos = camera.GetPos();
	//キーボードによる移動処理
	if (key->key[DIK_UP] >= 1) {
		if (cameraPos.Y < camera.GetTarget().Y + cameraMaxDist.Y) {
			cameraPos.Y += cameraSpeed;
		}
	}
	if (key->key[DIK_DOWN] >= 1) {
		if (cameraPos.Y > camera.GetTarget().Y - cameraMaxDist.Y) {
			cameraPos.Y -= cameraSpeed;
		}
	}
	if (key->key[DIK_RIGHT] >= 1) {
		if (cameraPos.X < camera.GetTarget().X + cameraMaxDist.X) {
			cameraPos.X += cameraSpeed;
		}
	}
	if (key->key[DIK_LEFT] >= 1) {
		if (cameraPos.X > camera.GetTarget().X - cameraMaxDist.X) {
			cameraPos.X -= cameraSpeed;
		}
	}

	camera.SetPos(cameraPos);
}
void CameraWork::Updata() {

	if (photoMode) {
		PhotoMode();
	}
	else  if (playerTarget) {
		PlayerTarget();
	}
	else {
		Move();
	}
	camera.Update();
}

void CameraWork::SetMoveNum(int _moveNum) {
	moveNumOld = moveNum;
	moveNum = _moveNum; 
	lerpCount = 0;
}