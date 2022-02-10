#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"


bool DammyWindow::SetMapTipPos() {
	int tmpX = x - 480 * size;
	int tmpY = y - 270 * size;
	for (int iy = 0; iy < 9; iy++) {
		for (int ix = 0; ix < 16; ix++) {
			mapTipPosOld[iy][ix].p[0] = mapTipPos[iy][ix].p[0];
			mapTipPosOld[iy][ix].p[1] = mapTipPos[iy][ix].p[1];

			mapTipPos[iy][ix].p[0] = tmpX + ix * 60 * size;
			mapTipPos[iy][ix].p[1] = tmpY + iy * 60 * size;

		}
	}

	return 1;
}

int DammyWindow::MoveGrab() {
	GameStatic* gs = GameStatic::GetInstance();
	Player* pl = Player::GetInstance();
	Cursor* cs = Cursor::GetInstance();

	int mouseX_EX = gs->MOUSE->x*gs->mouseExtend;
	int mouseY_EX = gs->MOUSE->y*gs->mouseExtend;

	// 箱の中で左クリックされている間moveGrabをtrue
	int magHitSizeX = 50;



	// 左クリックをやめたときフラグをfalse
	if (gs->MOUSE->left == 0) {
		grab = false;
	}
	if (x - (480) * size <= mouseX_EX &&
		x + (480) * size >= mouseX_EX &&
		y - (330)*size <= mouseY_EX &&
		y - (330 - magHitSizeX * 2)*size >= mouseY_EX) {
		cs->anime = true;
	}

	//掴み判定
	if ((pl->animeNum == STAY|| pl->animeNum == JUMP || pl->animeNum == RESPAWN) && gs->MOUSE->left == 1 &&
		x - (480 - magHitSizeX) * size <= mouseX_EX &&
		x + (480 - magHitSizeX) * size >= mouseX_EX &&
		y - (330)*size <= mouseY_EX &&
		y - (330 - magHitSizeX * 2)*size >= mouseY_EX) {

		actionStartMouseX = GetMouseX();
		actionStartMouseY = GetMouseY();

		moveXOld = x;
		moveYOld = y;

		pl->moveXOld = pl->x;
		pl->moveYOld = pl->y;

		grab = true;
	}
	//実際の移動
	if (grab == true) {

		x = moveXOld - (actionStartMouseX - GetMouseX())*gs->mouseExtend;
		y = moveYOld - (actionStartMouseY - GetMouseY())*gs->mouseExtend;

		if (pl->windowNum == num) {
			pl->x = pl->moveXOld - (actionStartMouseX - GetMouseX())*gs->mouseExtend;
			pl->y = pl->moveYOld - (actionStartMouseY - GetMouseY())*gs->mouseExtend;
		}

		//画面外判定
		if (0) {
			if (x - 480 * size <= 0) {
				x = 0 + 480 * size;
				pl->x = pl->xOld;
			}
			else if (x + 480 * size >= 1920) {
				x = 1920 - 480 * size;
				pl->x = pl->xOld;
			}
			if (y - 300 * size <= 0) {
				y = 0 + 300 * size;
				pl->y = pl->yOld;
			}
			else if (y + 300 * size >= 1080) {
				y = 1080 - 300 * size;
				pl->y = pl->yOld;
			}
		}

		return num;//掴んでいるDW番号を返す
	}


	return -1;//してないよー
}

void DammyWindow::HitMapToPlayer(DammyWindow* dw, int maxWindow) {
	GameStatic* gs = GameStatic::GetInstance();
	Player* pl = Player::GetInstance();
	Cursor* cs = Cursor::GetInstance();
	MapState* ms = MapState::GetInstance();


	Pos2 plRect;
	plRect.p[0] = pl->x;
	plRect.p[1] = pl->y;
	Pos2 plRectSize;
	plRectSize.p[0] = 60 * pl->size;
	plRectSize.p[1] = 180 * pl->size;

	if (num != pl->windowNum) {
		for (int my = 0; my < 9; my++) {
			for (int mx = 0; mx < 16; mx++) {
				switch (ms->state[mapTip.map[my][mx]]) {
				case NONE:case GOAL:case START_P:case SWITCH:case GOAL_KEY:
					continue;
					break;
				case DISAPPEAR_WALL:
					if (pl->switchButton)
						continue;
					break;
				case WALL:case THORN:	case FAN:
					break;
				}

				Pos2 rect;
				rect.p[0] = mapTipPos[my][mx].p[0] + 30 * size;
				rect.p[1] = mapTipPos[my][mx].p[1] + 30 * size;
				Pos2 rectOld;
				rectOld.p[0] = mapTipPosOld[my][mx].p[0] + 30 * size;
				rectOld.p[1] = mapTipPosOld[my][mx].p[1] + 30 * size;
				Pos2 rectSize;
				rectSize.p[0] = 60 * size;
				rectSize.p[1] = 60 * size;
				


				if (HitRectDouble(rect, rectSize, plRect, plRectSize)) {
					x = xOld;
					y = yOld;
					pl->x = pl->xOld;
					pl->y = pl->yOld;
					grab = false;
					pl->death = true;
				}
			}
		}
	}
	else {
		for (int win = 0; win < maxWindow; win++) {
			if (win == num)
				continue;
			for (int my = 0; my < 9; my++) {
				for (int mx = 0; mx < 16; mx++) {
					switch (ms->state[dw[win].mapTip.map[my][mx]]) {
					case NONE:case GOAL:case START_P:case SWITCH:case GOAL_KEY:
						continue;
						break;
					case DISAPPEAR_WALL:
						if (pl->switchButton)
							continue;
						break;
					case WALL:case THORN:	case FAN:
						break;
					}

					Pos2 rect;
					rect.p[0] = dw[win].mapTipPos[my][mx].p[0] + 30 * size;
					rect.p[1] = dw[win].mapTipPos[my][mx].p[1] + 30 * size;
					Pos2 rectOld;
					rectOld.p[0] = dw[win].mapTipPosOld[my][mx].p[0] + 30 * size;
					rectOld.p[1] = dw[win].mapTipPosOld[my][mx].p[1] + 30 * size;
					Pos2 rectSize;
					rectSize.p[0] = 60 * size;
					rectSize.p[1] = 60 * size;
					if (HitRectDouble(rect, rectSize, plRect, plRectSize)) {
						x = xOld;
						y = yOld;
						pl->x = pl->xOld;
						pl->y = pl->yOld;
						grab = false;
						pl->death = true;
					}
				}
			}
		}
	}

}

int DammyWindow::MagGrab(DammyWindow* dw, int maxWindow) {
	GameStatic* gs = GameStatic::GetInstance();
	Player* pl = Player::GetInstance();
	int mouseX_EX = gs->MOUSE->x*gs->mouseExtend;
	int mouseY_EX = gs->MOUSE->y*gs->mouseExtend;

	//拡縮

	int magHitSizeX = 50 * 2;
	// 左クリックをやめたときフラグをfalse

	if (gs->MOUSE->left == 0) {
		grabMag = false;
	}

	bool noMag = false;
	for (int i = 0; i < maxWindow; i++) {
		if (dw[i].grab) {
			noMag = true;
			break;
		}
	}
	if (pl->animeNum==STAY&&pl->stayCount>10&&noMag == false && gs->MOUSE->left >= 1 && y - (300)*size <= mouseY_EX && y - (300 - magHitSizeX)*size >= mouseY_EX) {
		if (
			(x + (480 - magHitSizeX) * size <= mouseX_EX && x + (480) * size >= mouseX_EX)) {

			if (grabMag == false) {

				grabMag = true;
				if (gs->MOUSE->left == 1) {
					DWtoPL_RX = pl->x - x;
					DWtoPL_RY = pl->y - y;
					oldSize = size;
					pl->oldSize = pl->size;


					oldMouseX = GetMouseX() * gs->mouseExtend;//マウス現在地
					oldMouseY = GetMouseY() * gs->mouseExtend;//マウス現在地
				}
			}
		}
	}



	//画面外判定
	if (x - 480 * size <= 0 || x + 480 * size >= 1920 || y - 300 * size <= 0 || y + 300 * size >= 1080) {
		return -1;
	}
	//拡縮の数値を変える
	if (grabMag == true) {

		float mx = GetMouseX()*gs->mouseExtend;//マウス現在地
		float my = GetMouseY()*gs->mouseExtend;//マウス現在地

		float dwRightPosX = x + 490 * size;
		float dwRightPosY = y - 320 * size;

		float sizeAnserX = mx / dwRightPosX;
		float sizeAnserY = dwRightPosY/my;


		size = size * sizeAnserX;


		if (size <= 0.25) {
			size = 0.25;
			
		}
		//プレイヤーのサイズも変更
		if (num == pl->windowNum) {
			pl->size = pl->oldSize + (size - oldSize);

			if (pl->size <= 0.25) {
				pl->size = 0.25;
			}
			int tmpX = x - 480 * size;
			int tmpY = y - 270 * size;

			pl->x = x + DWtoPL_RX * (size / oldSize);
			pl->y = y + DWtoPL_RY * (size / oldSize) - 5;
		}

		return 1;
	}

	return -1;
}


int DammyWindow::GameEnd(DammyWindow* dw, int maxWindow) {
	GameStatic* gs = GameStatic::GetInstance();
	Player* pl = Player::GetInstance();
	MapState* ms = MapState::GetInstance();
	
	//ゲームクリア
	if (pl->clear) {
		pl->animeNum = CLEAR;
		return 1;
	}
	//ゲームオーバー
	else if (pl->death) {
		pl->animeNum = DEATH;

		return 0;
	}


	return -1;
}

int DammyWindow::EndMove() {
	Pos2 winPos;
	winPos.p[0] = x;
	winPos.p[1] = y;

	const float speedR = 10;
	winPos = TransformPosition2(winPos, Pos2{ 960.0f,540.0f }, speedR);
	x = winPos.p[0];
	y = winPos.p[1];

	const float speedSize = 0.1f;
	float endSize = 0.75f;
	if (num == 0)
		endSize = 1.5f;
	TransformSize(&size, endSize, speedSize);

	if (num == 0 && size >= 1.5) {
		return 1;
	}
	return 0;
}

