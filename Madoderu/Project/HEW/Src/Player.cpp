#include "Game.h"

int Player::Move(DammyWindow* dw, int maxWindow) {
	GameStatic* gs = GameStatic::GetInstance();
	MapState* ms = MapState::GetInstance();

	GameImage* im = GameImage::GetInstance();
	GameSound* so = GameSound::GetInstance();

	//画面外死亡判定
	if (x <= 0||x>=1920||y<=0||y>=1080) {
		if (death == false) {
			so->SE[2].Play();
		}
		death = true;
	}
	//ジャンプ
	if ((gs->KEY->key[DIK_W] == 1 || gs->KEY->key[DIK_SPACE] == 1) && animeNum != JUMP) {
		animeF = 0;
		animeNum = JUMP;
		nowAnimePage = 0;
		//ジャンプ
		jump = true;
		float jumpPower = -37;
		gravity = jumpPower * size;

		gs->jumpTutolial = true;

	}

	const int jumpMoveGravityDefine = -5;
	//ダッシュと待機
	if (gs->KEY->key[DIK_A] >= 1) {
		so->SE[1].Play();
		gs->moveTutolial = true;
		if (jump == false && animeNum != DASH) {
			animeNum = DASH;
			animeF = 0;
			nowAnimePage = 0;
		}
		if (animeNum == JUMP) {
			if (gravity > jumpMoveGravityDefine) {
				x -= speed;
				if (speed < speedMax)
					speed += speed;
			}
		}
		else {
			x -= speed;
			if (speed < speedMax)
				speed += speed;
		}
		vectorLR = 1;
	}
	else if (gs->KEY->key[DIK_D] >= 1) {
		so->SE[1].Play();
		gs->moveTutolial = true;
		if (jump == false && animeNum != DASH) {
			animeNum = DASH;
			animeF = 0;
			nowAnimePage = 0;
		}

		if (animeNum == JUMP) {
			if (gravity > jumpMoveGravityDefine) {
				x += speed;
				if (speed < speedMax)
					speed += speed;
			}
		}
		else {
			x += speed;
			if (speed < speedMax)
				speed += speed;
		}
		vectorLR = 0;
	}
	else {
		if (jump == false && animeNum!=STAY) {
			animeNum = STAY;
			animeF = 0;
			nowAnimePage = 0;

		}
	}
	if (animeNum == STAY) {
		if (stayCount < 100)
			stayCount++;
	}
	else {
		stayCount = 0;
	}


	//重力
	if (jump == true) {
		y += gravity;
		if (gravity < gravityMax) {
			gravity += 5.0f*size;
		}
		else {
			gravity = gravityMax * size;
		}
	}

	Pos2 rectPL[4];
	for (int i = 0; i < 4; i++) {
		rectPL[i].p[0] = x - (60 * size) + i % 2 * 120 * size;

		rectPL[i].p[1] = y - 90 * size;
		if (i > 1)
			rectPL[i].p[1] = y + 90 * size;
	}

	//マップとの当たり判定
	bool hit = false;

	for (int win = 0; win < maxWindow; win++) {
		for (int _y = 0; _y < 9; _y++) {
			for (int _x = 0; _x < 16; _x++) {
				//チップごとの処理
				switch (ms->state[dw[win].mapTip.map[_y][_x]]) {
				case NONE:
				
				case START_P:

					continue;
					break;
				case DISAPPEAR_WALL:
					if (switchButton)
						continue;
				case WALL:
				case THORN:
				case GOAL_KEY:
				case SWITCH:
				case FAN:
				case GOAL:
					break;
				}

				Pos2 rect;
				rect.p[0] = dw[win].mapTipPos[_y][_x].p[0] + 30 * dw[win].size;
				rect.p[1] = dw[win].mapTipPos[_y][_x].p[1] + 30 * dw[win].size;
				Pos2 rectSwitch;
				rectSwitch.p[0] = dw[win].mapTipPos[_y][_x].p[0] + 90 * dw[win].size;
				rectSwitch.p[1] = dw[win].mapTipPos[_y][_x].p[1] + 30 * dw[win].size;

				Pos2 rectFan;
				rectFan.p[0] = dw[win].mapTipPos[_y][_x].p[0] + 30 * dw[win].size;
				rectFan.p[1] = dw[win].mapTipPos[_y][_x].p[1] + 30 * dw[win].size -270*dw[win].size;


				Pos2 rectSize;
				rectSize.p[0] = 60.0f * dw[win].size;
				rectSize.p[1] = 60.0f * dw[win].size;
				
				Pos2 rectSwitchSize;
				rectSwitchSize.p[0] = 180.0f * dw[win].size;
				rectSwitchSize.p[1] = 60.0f * dw[win].size;

				Pos2 rectGoal;
				rectGoal.p[0] = dw[win].mapTipPos[_y][_x].p[0] + 60 * dw[win].size;
				rectGoal.p[1] = dw[win].mapTipPos[_y][_x].p[1] + 90 * dw[win].size;
				Pos2 rectSizeGoal;
				rectSizeGoal.p[0] = 120.0f * dw[win].size;
				rectSizeGoal.p[1] = 180.0f * dw[win].size;

				Pos2 rectSizeFan;
				rectSizeFan.p[0] = 180.0f * dw[win].size;
				rectSizeFan.p[1] = 540.0f * dw[win].size;

				Pos2 player;
				player.p[0] = x;
				player.p[1] = y;

				Pos2 playerOld;
				playerOld.p[0] = xOld;
				playerOld.p[1] = yOld;


				Pos2 playerSize;
				playerSize.p[0] = 60 * size;
				playerSize.p[1] = 170 * size;

				switch (ms->state[dw[win].mapTip.map[_y][_x]]) {
				case WALL:
				case DISAPPEAR_WALL:

					if (HitRectDouble(rect, rectSize, player, playerSize)) {
						player = HitRectAllProcess(rect, rectSize, player, playerOld, playerSize);
						x = player.p[0];

						player.p[1] = y;
						player = HitRectAllProcess(rect, rectSize, player, playerOld, playerSize);		
						y = player.p[1];
		
						//地面判定
						if (y + playerSize.p[1] / 2 < rect.p[1] - rectSize.p[1] / 2) {
							jump = false;
							hit = true;
						}

					}
					break;
				case FAN:
					if (HitRectDouble(rectFan, rectSizeFan, player, playerSize)) {
						animeNum = JUMP;
						gravity = 0;
						y -= 11;//実際に浮く移動  
					}
					break;
				case THORN:
					if (HitRectDouble(rect, rectSize, player, playerSize)) {
						death = true;
					}
					break;
				case SWITCH:
	
					Pos2 old;
					old.p[0] = x;
					old.p[1] = yOld;

					if (HitRectDouble(rectSwitch, rectSwitchSize, player, playerSize)) {

						if ((gs->KEY->key[DIK_W] == 1 || gs->KEY->key[DIK_SPACE] == 1)) {
								switchButton = !switchButton;
								so->SE[4].Play();
							}
						}

					
					break;
				case GOAL_KEY:
					if (HitRectDouble(rect, rectSize, player, playerSize)) {
						if (key == false) {
							so->SE[6].Play();
						}
						key = true;
						dw[win].mapTip.map[_y][_x] = NONE;
					}
					break;
				case GOAL:
					if (key&&HitRectDouble(rectGoal, rectSizeGoal, player, playerSize)) {
						if (clear == false) {
							so->SE[2].Play();
						}
						clear = true;
					}
					break;

				}


			}
		}
	}


	if (hit == false) {
		jump = true;
	}
	xOld = x;
	yOld = y;



	return 1;
}


int Player::Anime() {
	//アニメ変移処理
	if (animeNumOld != animeNum) {
		animeF = 0;
		nowAnimePage = 0;
		animeNumOld = animeNum;
	}
	animeF++;

	nowAnimePage = animeF / animeNumOnNumChangeFrame[animeNum];


	if (nowAnimePage >= animeNumOnNum[animeNum]) {

		if (animeNum == JUMP) {
			nowAnimePage--;

			animeF-=10;
		}
		else {
			animeF = 0;
			nowAnimePage = 0;
		}
	}

	return 2;
}

