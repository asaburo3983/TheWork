#include "MainGame.h"

int clearStage = 0;
void SetClearStage(int num) {
	clearStage = num;
}
class TitleWindow {
public:
	float x, y;
	float sizeX = 1, sizeY = 1;

	int selectEasingTime = 0;
	float selectEasingR = 0;


	int touchPopTime = 0;
	float touchPopSize = 0;
public:
	void Reset();
};
void TitleWindow::Reset() {

	sizeX = 1;
	sizeY = 1;

	selectEasingTime = 0;
	selectEasingR = 0;

	touchPopTime = 0;
	touchPopSize = 0;
}
TitleWindow tw[3];

//�ŏ��̃^�C�g�����S�̈ړ�������g�k�����Ŏg��
int easingTime = 0;
float easingR = 0;

bool decisionSelect = false;		//���莞����

float decisionSelectSize = 1.0f;
int decisionSelectTime = 0;

void StageSelectLoad() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	//������
	for (int i = 0; i < 3; i++) {
		tw[i].x = 960 - 960 / 7.8 + i % 2 * (960 / 7.8*2);
		if (i < 2)
			tw[i].y = 540 - 540 / 4.5;
		else
			tw[i].y = 540 + 540 / 4.5;

		tw[i].Reset();
	}

	
	easingTime = 0;
	easingR = 0;

	decisionSelect = false;
	decisionSelectSize = 1.0f;
	decisionSelectTime = 0;

	gs->stageSelect = -1;
}
void StageSelectUpdata() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();
	GameSound* so = GameSound::GetInstance();

	//���߂̃C�[�W���O����
	if (easingTime < 90) {
		easingTime += 5;
		easingR = GetCosWave(easingTime) * 30;
	}
	else {
		easingR = 0;
	}

	//�N���b�N����
	if (gs->MOUSE->left == 1 && decisionSelect==false) {

		//�I��
		for (int i = 0; i < 3; i++) {

			//�D��I���i�đI���j
			if (gs->stageSelect != -1 && im->stageSelect[gs->stageSelect+4].Hit(gs->MOUSE->x, gs->MOUSE->y)) {
				if (gs->stageSelect <= clearStage) {
					decisionSelect = true;
				}
				else {
					so->SE[7].Play();
				}
				break;
			}
			//�ʏ�I��
			if (im->stageSelect[i+4].Hit(gs->MOUSE->x, gs->MOUSE->y)) {
				if (gs->stageSelect==-1||tw[gs->stageSelect].selectEasingTime == 90) {
					gs->stageSelect = i;
					tw[i].touchPopTime = 1;
				}
				break;
			}
		}
	}



	//���莞�A�S�̏k��(�I�𕨈ȊO)
	if (decisionSelect&&decisionSelectSize >= 0) {
		decisionSelectSize -= 0.05f;
	}
	//���肵�Ă���̎��ԂŃV�[���ړ�
	if (decisionSelect) {
		if (decisionSelectTime < 90)
			decisionSelectTime += 7;

		decisionSelectSize = GetCosWave(decisionSelectTime);

		//���̎��Ԉȍ~�ɕ��U���ăQ�[�����n�܂�
		if (decisionSelectTime >= 90) {

			//�ڍs�̍ۂɑ��ʃE�B���h�E�̒l��n���Ă���
			gs->scene = GAME;
			GameLoad();
		}


	}


	//POP�C�[�W���O�����A�I�����ꂽ�E�B���h�E�̈ړ�����
	for (int i = 0; i < 3; i++) {
		//POP�C�[�W���O

		//�ŏ��̃^�C�g�����S�@�ړ�
		int tmpAng = 15;
		if (i < 2)
			GetCirclePoint(tw[i].x, tw[i].y, easingR, (225 - tmpAng) + (90 + tmpAng * 2)*i + 90, &tw[i].x, &tw[i].y);
		else
			GetCirclePoint(tw[i].x, tw[i].y, easingR, (135 + tmpAng) - (90 + tmpAng * 2)*(i - 2) + 90, &tw[i].x, &tw[i].y);

		//�ŏ��̃^�C�g�����S�@�T�C�Y�ύX
		if (easingTime >= 90) {
			//tw[i].sizeX = 0;
			tw[i].sizeY;
		}
		tw[i].sizeX = 1 + 3.0*easingTime / 90.0 * 0.75;
		tw[i].sizeY = 1 + 1.25*easingTime / 90.0 * 0.75;

		//�I�����ꂽ���@�E�B���h�E�̊g�k���o
		if (tw[i].touchPopTime > 0) {

			tw[i].touchPopSize = GetCosWave(tw[i].touchPopTime)*0.3;
			tw[i].touchPopTime += 5;
			if (tw[i].touchPopTime >= 90) {
				tw[i].touchPopTime = 0;
				tw[i].touchPopSize = 0;
			}
		}

		////�I�����ꂽ���@�E�B���h�E�̈ړ�����
		if (i == gs->stageSelect) {

			if (tw[i].selectEasingTime < 90) {
				int tmpAng = 15;
				tw[i].selectEasingTime += 5;
				tw[i].selectEasingR = GetCosWave(tw[i].selectEasingTime) * 45;
				if (i < 2)
					GetCirclePoint(tw[i].x, tw[i].y, tw[i].selectEasingR, 90 + (225 - tmpAng - 180) + (90 + tmpAng * 2)*i, &tw[i].x, &tw[i].y);
				else
					GetCirclePoint(tw[i].x, tw[i].y, tw[i].selectEasingR, 90 + (135 + tmpAng - 180) - (90 + tmpAng * 2)*(i - 2), &tw[i].x, &tw[i].y);
			}
		}
		else if (i != gs->stageSelect&& gs->stageSelect != -1) {
			if (tw[i].selectEasingTime >= 90) {
				int tmpAng = 15;
				tw[i].selectEasingTime += 5;
				tw[i].selectEasingR = GetCosWave(tw[i].selectEasingTime) * 45;
				if (i < 2)
					GetCirclePoint(tw[i].x, tw[i].y, tw[i].selectEasingR, 90 + (225 - tmpAng - 180) + (90 + tmpAng * 2)*i, &tw[i].x, &tw[i].y);
				else
					GetCirclePoint(tw[i].x, tw[i].y, tw[i].selectEasingR, 90 + (135 + tmpAng - 180) - (90 + tmpAng * 2)*(i - 2), &tw[i].x, &tw[i].y);
				if (tw[i].selectEasingTime > 170) {
					tw[i].selectEasingTime = 0;
					tw[i].selectEasingR = 0;
				}
			}
		}

	}


}

int rollTime = 0;
const int rollSpeed = 4;
void StageSelectDraw() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();
	if (rollTime >= 1920) {
		rollTime = 0;
	}
	rollTime+=rollSpeed;


	//�X�e�[�W�摜
	for (int i = 0; i < 3; i++) {
		float tmpAlpha = 1.0 * (float)easingTime / 90.0;
		if (tmpAlpha <= 0.001)
			break;

		if (i == gs->stageSelect)
			continue;

		float sizeX = (0.75 * (tw[i].sizeX / 3.125) + tw[i].touchPopSize) * decisionSelectSize;
		float sizeY = (0.75 * (tw[i].sizeY / 1.88541663) + tw[i].touchPopSize) * decisionSelectSize;

		im->stageSelect[i+4].DrawEX(tw[i].x, tw[i].y, 0, sizeX, sizeY, tmpAlpha, 1, false, false);


		im->staticIm[0].DrawEX(tw[i].x, tw[i].y, 0, sizeX, sizeY, tmpAlpha);

	}
	im->stageSelect[8].Draw(1450, 800);
		if (gs->stageSelect != -1) {
			float decisionSelectSizeBase = 2;

			float sizeX = (0.75*(tw[gs->stageSelect].sizeX / 3.125) + tw[gs->stageSelect].touchPopSize)*(decisionSelectSizeBase - decisionSelectSize);
			float sizeY = (0.75*(tw[gs->stageSelect].sizeY / 1.88541663) + tw[gs->stageSelect].touchPopSize)*(decisionSelectSizeBase - decisionSelectSize);

			im->stageSelect[gs->stageSelect + 4].DrawEX(tw[gs->stageSelect].x, tw[gs->stageSelect].y, 0, sizeX, sizeY, 1, 1, false, false);

			im->stageSelect[gs->stageSelect].DrawEX(tw[gs->stageSelect].x +960* sizeX, tw[gs->stageSelect].y, 0, sizeX, sizeY, 1, 1, false, false, RECT{ 0+rollTime,0,960+rollTime,540 });

			im->staticIm[0].DrawEX(tw[gs->stageSelect].x, tw[gs->stageSelect].y, 0, sizeX, sizeY, 1);
		}



	//�^�C�g���摜
	for (int i = 0; i < 3; i++) {
		float tmpAlpha = 1.0 - (((float)easingTime + 1) / 90.0);
		if (tmpAlpha <= 0.001)
			break;
		im->title[i].DrawEX(tw[i].x, tw[i].y, 0, tw[i].sizeX, tw[i].sizeY, tmpAlpha);
	}
}