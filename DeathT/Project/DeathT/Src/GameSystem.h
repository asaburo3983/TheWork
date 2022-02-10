#pragma once
class GameSystem {
public:
	static GameSystem* GetInstance() {
		static GameSystem bs;
		return &bs;
	}
	void Reset(int _stageNum) {
		stageNum = _stageNum;//ステージ番号

		cameraDistY = 0.0f;//カメラ移動
		cameraBossCount = 0;//カメラのボスで使う？イミフ

		pause = false;
		pauseSelectConfirm = false;
	}
public:
	int stageNum = 0;//ステージ番号

	float cameraDistY = 0.0f;//カメラ移動
	int cameraBossCount = 0;//カメラ移動で使う

	bool pause = false;
	bool pauseSelectConfirm = false;
};