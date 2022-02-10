#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include <vector>

bool StartMove();
bool EndMove(int clear);

//���������K�v�Ȃ���
DammyWindow* dw;
int startDashMovie = 0;
const int startDashMovieEnd = 180;

int gameoverMovie = 1000;
const int gameoverMovieEnd = 20;

int maxWindow = 0;
bool startGame = true;
int startEasingTime = 0;
int nowStageOnStage = 0;

int startCount = 0;
int endCount = 0;

int effectCount = 0;

float redEffectCount = 0;

class TextManager {
public:
	void StartUp() {
		draw = 0;
		page = 0;
		sizeY = 0;
		textCount = 1;
	}
public:
	int draw = 0;
	int page = 0;
	float sizeY = 0;
	const float sizeSpeed = 0.1f;
	float textCount = 1;
	float textSpeed = 0.2f;
	wchar_t text[200][100]{
		//Stage0-0
		{L"���[���A���̕������ǂ߂�H"},
		{L"�����ǂ߂�悤�Ȃ玄�̎w���ɂ��������ă{�^����������"},
		{L"Move[A�L�[orD�L�[]�@Jump[Space�L�[]"},//3
		//�����
		{L"�悩�����A���͑��u�͐����Ă�݂����ˁI"},
		{L"�P�������ɂ�����A�������̑��̐��E����o���Ăق�����"},
		{L"���͉�ʂ𑀂����肵�ČN�̃T�|�[�g�͂ł��邯��"},
		{L"�N�݂����Ƀ}�E�X�𑀍삵����ł��Ȃ���"},
		{L"������N���u���v�Ɓu���v�𑀍삵�āI"},
		{L"��n�߂Ɂu���v�̏㕔�����}�E�X�ł���ő��삵�Ă݂�"},//8
		//�����
		{L"���񂤂�A���������I"},
		{L"�u���v�̂ǂ����ɗ����Ă錮�����Əo���ɃA�N�Z�X�ł����"},//10
		//Stage0-1
		{L"������[�A�E�o�[�I�I"},
		{L"���āA�܂�����肶��Ȃ��́I�H"},
		{L"���߂�I����������Ǝ��ɕt�������āI"},
		{L"���[���ƁA���̃X�e�[�W�͂�����Ɠ�����"},
		{L"�m���Ă邩���m��Ȃ����ǁu���v�̉E�������ő��삷���"},
		{L"���̑傫����ς��邱�Ƃ��ł����"},
		{L"�������鑋������������Ǝ����ꏏ�ɏ������Ȃ��"},
		{L"�H�v����΂ǂ��ɂł������邩���I"},//18
		//Stage0-2
		{L"��{�I�ȑ���͂���őS����"},
		{L"�܂������������玄���A�V�X�g�����I"},//20
		
		//Stage1-0
		{L"���ԂȂ��I"},
		{L"���肪�����炯����"},
		{L"����������ɂ�����ς݂����ɂȂ���ˁA�A�A"},//23
		//Stage1-1
		{L"�ǂ����Ă���Ȃɓ����������̂�����H"},
		{L"�o���ɍs���̂����J�ˁA���ЂƂ肶�ᖳ����������I"},//25
		//Stage1-2
		{L"���ԂȂ��I�n�ʂ��Ȃ��I�I"},//26

		//Stage2-0
		{L"�������͂Ђ���Ƃ������"},//27
		{L"�܂����n�ʂ��Ȃ��Ƃ͂ˁA�A�A"},//28
		{L"����͂����Ƃ���͉�������H�X�C�b�`�H"},//29
		{L"�W�����v����Ή��������ˁI"},//30

		//Stage2-1
		{L"�܂����I?�ł��A����́A�A�A"},//31
		 
		{L"�ǂ���炱��ōŌ���ۂ����"},
		{L"��Ɍ����Ă������ǂ��肪�Ƃ�"},
		{L"�N�����Ȃ������炱���܂ł���Ȃ���������"},//34
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},




		{L""},		
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},		
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},
		{L""},


	};

};
TextManager tm;

bool tutorialGrab = false;

void GameLoad() {
	startCount = 0;
	endCount = 0;
	redEffectCount = 0;

	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();

	tm.StartUp();
	switch (gs->stageSelect) {
	case 0:
		switch (nowStageOnStage) {
		case 0:
			tm.page = 0;
			break;
		case 1:
			tm.page = 11;
			break;
		case 2:
			tm.page = 19;
			break;
		}
		break;
	case 1:
		switch (nowStageOnStage) {
		case 0:
			tm.page = 21;
			break;
		case 1:
			tm.page = 24;
			break;
		case 2:
			tm.page = 26;
			break;
		}
		break;
	case 2:
		switch (nowStageOnStage) {
		case 0:
			tm.page = 27;
			break;
		case 1:
			tm.page = 31;
			break;
		case 2:
			tm.page = 32;
			break;
		}
		break;

		//���Ԃ���Ǝv���܂�
	case 3:
		break;

	}
	const int maxWindowList[4][3]{
		{2,2,2},//�M�~�b�N�Ȃ�
		{2,2,3},//���M�~�b�N
		{2,3,3},//�X�C�b�`�M�~�b�N
	};
	maxWindow = maxWindowList[gs->stageSelect][nowStageOnStage];

	dw = new DammyWindow[maxWindow];
	for (int i = 0; i < maxWindow; i++) {
		dw[i].num = i;
		dw[i].x = 960;
		dw[i].y = 540;
		dw[i].size=0.75f;
		dw[i].scrollTime = 0;
		char filePath[50];
		sprintf(filePath, "Data/Stage/Stage%d/%d/save%d.dat", gs->stageSelect, nowStageOnStage, i);
		MapTip mt;
		LoadMap(&mt, filePath);
		dw[i].mapTip = mt;
	}

	dw[0].size=1.5f;

	Player* pl = Player::GetInstance();
	pl->x = (960-480);
	pl->y = 540+270/1.5;

	
}


void GameUpdata() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();
	Player* pl = Player::GetInstance();
	Cursor* cs = Cursor::GetInstance();
	cs->anime = false;

	GameSound* so = GameSound::GetInstance();
	if (gs->stageSelect >= 0) {
		so->BGM[gs->stageSelect + 1].Play();
	}

	if (startDashMovie <= startDashMovieEnd) {
		startDashMovie++;
		pl->animeNum = DASH;
		pl->size = 1.5f;
		pl->Anime();
		pl->x = TransformPosition1(pl->x, 960 + 480, 960.0f*1.5f / startDashMovieEnd);
		//�ύX�����T�C�Y�̏�����
		if (startDashMovie > startDashMovieEnd) {
			pl->animeNum = STAY;
			pl->size = 0.75f;
			//TODO:�v���C���[�̈ʒu�Z�b�g
		}
	}

	if (gameoverMovie <= gameoverMovieEnd) {
		gameoverMovie++;


	}

	//�Q�[���̃��C���쓮
	if (startDashMovie > startDashMovieEnd && gameoverMovie >= gameoverMovieEnd) {
		if (StartMove() == false) {


			if (startCount < 30) {
				startCount++;
				pl->animeNum = RESPAWN;
				so->SE[2].Play();
			}
			static bool gameStop = false;


			if (gameStop == false && endCount == 0 && startCount >= 30) {

				//���͂�؂�
				if (tm.draw != 0) {
					gs->KEY->key[DIK_W] = 0;
					gs->KEY->key[DIK_SPACE] = 0;
					gs->KEY->key[DIK_A] = 0;
					gs->KEY->key[DIK_S] = 0;
					gs->KEY->key[DIK_D] = 0;
				}
				if (tm.draw != 0 && (tm.page == 26 ||tm.page==31)) {
					
				}
				else {
					pl->Move(dw, maxWindow);
				}
				//�_�~�[�E�B���h�E
				//�g�k
				bool setWinNum = true;

				for (int i = 0; i < maxWindow; i++) {
					int check = dw[i].MagGrab(dw, maxWindow);
					if (check != -1) {
						setWinNum = false;
						break;
					}
				}
				//�ړ�		
				bool grab = false;
				for (int i = 0; i < maxWindow; i++) {
					int check = dw[i].MoveGrab();

					if (check != -1) {
						setWinNum = false;
						grab = true;
						tutorialGrab = true;
						break;
					}

				}
				for (int i = 0; i < maxWindow; i++) {
					if (grab) {
						dw[i].HitMapToPlayer(dw,maxWindow);
					}
				}
				pl->xOld = pl->x;
				pl->yOld = pl->y;

				for (int i = 0; i < maxWindow; i++) {

					dw[i].xOld= dw[i].x;
					dw[i].yOld= dw[i].y;

				}

				if (setWinNum) {
					int oldwinNum = pl->windowNum;
					bool set[4] = { false,false,false,false };
					for (int win = 0; win < maxWindow; win++) {
						if (HitRectDouble(Pos2{ (float)dw[win].x,(float)dw[win].y }, Pos2{ dw[win].size*980.0f,dw[win].size*640.0f }, Pos2{ (float)pl->x,(float)pl->y }, Pos2{ 60.0f*pl->size,180.0f*pl->size})) {
							pl->windowNum = win;
							set[win] = true;
						}
					}
					if (set[oldwinNum]) {
						pl->windowNum = oldwinNum;
					}

				}
			}

			for (int i = 0; i < maxWindow; i++) {
				int check = dw[i].GameEnd(dw, maxWindow);

				gameStop = EndMove(check);
			}

			cs->Anime();
				pl->Anime();
		}
	}
	//�}�b�v�`�b�v�̈ʒu���v�Z���ۑ�
	for (int i = 0; i < maxWindow; i++) {
		dw[i].SetMapTipPos();
	}
}

const int scrollSpeed = 2;

int startBackTime = 0;
void GameDraw() {
	GameStatic* gs = GameStatic::GetInstance();
	GameImage* im = GameImage::GetInstance();
	Player* pl = Player::GetInstance();
	MapState* ms = MapState::GetInstance();
	Cursor* cs = Cursor::GetInstance();
	GameSound* so = GameSound::GetInstance();


	//�w�i
	for (int i = 0; i < maxWindow; i++) {
		if (startDashMovie <= startDashMovieEnd && i != 0) {
			break;
		}
		im->stageSelect[gs->stageSelect].DrawEX(dw[i].x + 960 * dw[i].size, dw[i].y, 0, dw[i].size, dw[i].size, 0.75, 1, false, false, RECT{ 0 + dw[i].scrollTime,0,960 + dw[i].scrollTime,540 });

	}
	for (int i = 0; i < maxWindow; i++) {
		if (startDashMovie <= startDashMovieEnd && i != 0) {
			break;
		}
		//�v���C���[������E�B���h�E�����X�N���[�����ɂ߁[�������s��
		if (i == pl->windowNum) {
			if (dw[i].scrollTime >= 1920) {
				dw[i].scrollTime = 0;
			}
			dw[i].scrollTime += scrollSpeed;
			im->stageSelect[gs->stageSelect].DrawEX(dw[i].x + 960 * dw[i].size, dw[i].y, 0, dw[i].size, dw[i].size, 1, 1, false, false, RECT{ 0 + dw[i].scrollTime,0,960 + dw[i].scrollTime,540 });

		}
	}
	//�}�b�v�`�b
	if (effectCount < 59) {
		effectCount++;
	}
	else {
		effectCount = 0;
	}
	if (redEffectCount < 100) {
		redEffectCount+=2;
	}
	else {
		redEffectCount = 0;
	}
	for (int i = 0; i < maxWindow; i++) {
		for (int iy = 0; iy < 9; iy++) {
			for (int ix = 0; ix < 16; ix++) {
				if (ms->state[dw[i].mapTip.map[iy][ix]] == NONE || ms->state[dw[i].mapTip.map[iy][ix]] == START_P || (ms->state[dw[i].mapTip.map[iy][ix]] == DISAPPEAR_WALL && pl->switchButton)) {
					continue;
				}
				if (ms->state[dw[i].mapTip.map[iy][ix]] == FAN) {
					im->effect[effectCount].DrawEX(dw[i].mapTipPos[iy][ix].p[0], dw[i].mapTipPos[iy][ix].p[1] - 540 * dw[i].size, 0, dw[i].size, dw[i].size + 2, 1, 0);
					continue;
				}

				if (ms->state[dw[i].mapTip.map[iy][ix]] == SWITCH &&pl->switchButton==true) {
					im->mapTipIm[10].Draw(dw[i].mapTipPos[iy][ix].p[0], dw[i].mapTipPos[iy][ix].p[1], 0, dw[i].size, 1, 0);
					continue;

				}

				if (ms->state[dw[i].mapTip.map[iy][ix]] == WALL) {
					int tmp = dw[i].mapTip.map[iy][ix];
					switch (gs->stageSelect) {
						case 0:		
							break;
						case 1:
							tmp += 10;
							break;
						case 2:
							tmp += 11;
							break;
					}
					im->mapTipIm[tmp].Draw(dw[i].mapTipPos[iy][ix].p[0], dw[i].mapTipPos[iy][ix].p[1], 0, dw[i].size, 1, 0);
				}
				else {
					im->mapTipIm[dw[i].mapTip.map[iy][ix]].Draw(dw[i].mapTipPos[iy][ix].p[0], dw[i].mapTipPos[iy][ix].p[1], 0, dw[i].size, 1, 0);
				}
				
				//���܂�Ă���E�B���h�E�����鎞
				bool grab=false;
				for (int i = 0; i < maxWindow; i++) {
					if (dw[i].grab == true) {
						grab = true;
						break;
					}
				}
					
					
				//�v���C���[������E�B���h�E�ȊO��Ԃ����炷
				if (grab &&i != pl->windowNum) {
					im->mapTipIm[9].Draw(dw[i].mapTipPos[iy][ix].p[0], dw[i].mapTipPos[iy][ix].p[1], 0, dw[i].size, redEffectCount/200.0f, 0);
				}
				
			
			}
		}
	}

	//�X�^�[�g�O �w�i
	if (startDashMovie <= startDashMovieEnd) {
		if (startBackTime >= 1920) {
			startBackTime = 0;
		}
		startBackTime += scrollSpeed;
		
		im->stageSelect[gs->stageSelect].DrawEX( 960+960*1.5 , 540, 0,1.5, 1.5, 1, 1, false, false, RECT{ 0 + startBackTime,0,960 + startBackTime,540 });

	}
	

	//�v���C���[
	im->player[pl->animeNum][pl->nowAnimePage].DrawEX(pl->x, pl->y, 0, pl->size*0.62, pl->size*0.62,1,1,pl->vectorLR);


	//�E�B���h�E�t���[��
	for (int i = 0; i < maxWindow; i++) {
		if (startDashMovie <= startDashMovieEnd&&i!=0) {
			break;
		}
		if (pl->windowNum == i) {
			im->staticIm[1].Draw(dw[i].x, dw[i].y, 0, dw[i].size, 1);
		}
		else {
			im->staticIm[0].Draw(dw[i].x, dw[i].y, 0, dw[i].size, 1);
		}
	}

	int mouseX_EX = gs->MOUSE->x*gs->mouseExtend;
	int mouseY_EX = gs->MOUSE->y*gs->mouseExtend;

	//�ړ��`���[�g���A��
	if (tm.page == 3 && gs->stageSelect == 0 && nowStageOnStage == 0) {
		im->staticIm[3 + gs->jumpTutolial].Draw(pl->x - 120, pl->y - 120, 0, 2);
		im->staticIm[5 + gs->moveTutolial].Draw(pl->x - 120, pl->y - 240, 0, 2);
	}

	//���擾�pUI
	im->staticIm[8].Draw(1830, 930);

	if (pl->key) {
		im->mapTipIm[8].Draw(1830, 930,0,1.5);
	}
	//�e�L�X�g�\��
	if (startDashMovie == 180) {
		tm.draw = 1;
	}
	if (tm.draw < 60 && tm.draw!=0) {
		tm.draw++;
	}
	if(tm.draw==60&& tm.page!=-1)
	{
		if (gs->MOUSE->left == 1) {
			//�e�L�X�g�̒��f����
			switch (tm.page) {
			case 2:
			case 8:				
			case 10:
			case 18:
			case 20:
			case 23:
			case 25:
			case 26:
			case 30:
			case 31:
			case 34:
				tm.draw = 0;
			
				break;
			}

			tm.page++;
			tm.textCount = 1;
		}

		if (tm.sizeY <= 1) {
			tm.sizeY += tm.sizeSpeed;
		}
		im->staticIm[7].DrawEX(960,930,0,1,tm.sizeY);
		

		if (tm.textCount <= wcslen(tm.text[tm.page])+1) {
			tm.textCount+=tm.textSpeed;
		}
		wchar_t exportText[100];
		swprintf(exportText, (int)tm.textCount, tm.text[tm.page]);
		DrawString(exportText, 350, 930, StrColor{ 1,1,1,1 }, 3);
	}
	else if(tm.draw==0){
		if (tm.sizeY > 0) {
			tm.sizeY -= tm.sizeSpeed;
			im->staticIm[7].DrawEX(960, 930, 0, 1, tm.sizeY);
		}

		//�e�L�X�g���f����̕��A����
		if (tm.page == 3 && gs->moveTutolial&& gs->jumpTutolial) {
			tm.draw = 60;
		}
		if (tm.page == 9 && tutorialGrab && gs->MOUSE->left == 0) {
			tm.draw = 60;
		}
	}

	//�J�[�\��
	if (cs->anime) {
		im->cursorIm[cs->page].Draw(mouseX_EX, mouseY_EX,0,2);
	}
	//�t�F�[�h
	if (gameoverMovie <= gameoverMovieEnd) {
		im->staticIm[2].Draw(960, -540+ gameoverMovie*10, 0, 1, 0.9f);
		im->staticIm[2].Draw(960, 540+ gameoverMovie*10, 0, 1, 0.9f);
		so->SE[3].Play();
	}


}


bool StartMove() {
	MapState* ms = MapState::GetInstance();
	Player* pl = Player::GetInstance();

	//���߂̏k��	//���߂̈ړ�
	if (startGame) {
		dw[0].size -= 0.05f;
		if (dw[0].size <= 0.75f) {
			startGame = false;
			dw[0].size = 0.75f;
		}
	}
	if (startEasingTime <= 90) {
		startEasingTime += 5;

		switch (maxWindow) {
		case 1:
			dw[0].x = 960;
			break;
		case 2:
			dw[0].x = 960 - (1 - GetCosWave(startEasingTime)) * 480;
			dw[1].x = 960 + (1 - GetCosWave(startEasingTime)) * 480;

			break;
		case 3:

			dw[0].y = 540 - (1 - GetCosWave(startEasingTime)) * 270;
			dw[1].y = 540 + (1 - GetCosWave(startEasingTime)) * 270;
			dw[2].y = 540 + (1 - GetCosWave(startEasingTime)) * 270;

			dw[1].x = 960 - (1 - GetCosWave(startEasingTime)) * 480;
			dw[2].x = 960 + (1 - GetCosWave(startEasingTime)) * 480;

			break;
		case 4:
			dw[0].y = 540 - (1 - GetCosWave(startEasingTime)) * 270;
			dw[1].y = 540 - (1 - GetCosWave(startEasingTime)) * 270;
			dw[2].y = 540 + (1 - GetCosWave(startEasingTime)) * 270;
			dw[3].y = 540 + (1 - GetCosWave(startEasingTime)) * 270;

			dw[0].x = 960 - (1 - GetCosWave(startEasingTime)) * 480;
			dw[1].x = 960 + (1 - GetCosWave(startEasingTime)) * 480;
			dw[2].x = 960 - (1 - GetCosWave(startEasingTime)) * 480;
			dw[3].x = 960 + (1 - GetCosWave(startEasingTime)) * 480;

			break;
		}

		if (startEasingTime > 90) {

			for (int _y = 0; _y < 9; _y++) {
				for (int _x = 0; _x < 16; _x++) {
					if (ms->state[dw[0].mapTip.map[_y][_x]] == START_P) {
						Pos2 rect;
						rect.p[0] = dw[0].mapTipPos[_y][_x].p[0] + 60 * dw[0].size;
						rect.p[1] = dw[0].mapTipPos[_y][_x].p[1] + 90 * dw[0].size;
						pl->x = rect.p[0];
						pl->y = rect.p[1];
						pl->y -= 10;
						_x = 160;
						_y = 90;
					}
				}
			}
			
		}
		return true;
	}
	else {


		return false;
	}

}

bool EndMove(int clear) {

	//�ǂ���ł��Ȃ���
	if (clear == -1)
		return false;

	GameStatic* gs = GameStatic::GetInstance();
	Player* pl = Player::GetInstance();
	GameSound* so = GameSound::GetInstance();

	for (int i = 0; i < maxWindow; i++) {
		if (clear && endCount > 60) {
			if (dw[i].EndMove()) {
				delete dw;
				startDashMovie = 0;
				maxWindow = 0;
				startGame = true;
				startEasingTime = 0;

				pl->Reset();
				nowStageOnStage++;
				if (nowStageOnStage > 2) {
					so->BGM[gs->stageSelect+1].Stop();
					nowStageOnStage = 0;
					//�X�e�[�W�Z���N�g�Ɉړ�
					if (gs->stageSelect == 2) {
						CreditLoad();
						gs->scene = CREDIT;
					}
					else {
						SetClearStage(gs->stageSelect + 1);
						StageSelectLoad();
						gs->scene = STAGESELECT;
					}
					return true;
				}
				GameLoad();
				return true;
			}
		}
			else if (clear == false&&endCount>120) {
				if (dw[i].EndMove()) {
					delete dw;
					gameoverMovie = 0;
					maxWindow = 0;
					startGame = true;
					startEasingTime = 0;

					pl->Reset();
					GameLoad();
					return true;

				}
				
			}
			
		}

	endCount++;
	return false;
}
