#pragma once
class GameSystem {
public:
	static GameSystem* GetInstance() {
		static GameSystem bs;
		return &bs;
	}
	void Reset(int _stageNum) {
		stageNum = _stageNum;//�X�e�[�W�ԍ�

		cameraDistY = 0.0f;//�J�����ړ�
		cameraBossCount = 0;//�J�����̃{�X�Ŏg���H�C�~�t

		pause = false;
		pauseSelectConfirm = false;
	}
public:
	int stageNum = 0;//�X�e�[�W�ԍ�

	float cameraDistY = 0.0f;//�J�����ړ�
	int cameraBossCount = 0;//�J�����ړ��Ŏg��

	bool pause = false;
	bool pauseSelectConfirm = false;
};