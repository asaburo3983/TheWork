#pragma once
#include "MainGame.h"


using namespace GameMath;
enum MAP_STATE {
	/*メインシステム*/	NONE, WALL, GOAL, START_P,

	/*ギミック*/		THORN, SWITCH, DISAPPEAR_WALL, FAN, 
	
						GOAL_KEY
};
class MapState {
public:
	static MapState* GetInstance() {
		static MapState ms;
		return &ms;
	}
	const int state[20]{
	NONE,WALL,GOAL,START_P,
	THORN, SWITCH, DISAPPEAR_WALL, FAN, 
	GOAL_KEY
	};
};

class DammyWindow {

public:
	struct MapTip mapTip;

	

	int x = 0;
	int y = 0;
	struct Pos2 mapTipPos[9][16];
	struct Pos2 mapTipPosOld[9][16];
	
	int xOld;
	int yOld;

	int moveXOld = NULL;//移動用のの１F前座標
	int moveYOld = NULL;

	float size=0;

	int DWtoPL_RX = 0;
	int DWtoPL_RY = 0;
	float oldSize = 1;
	int num = NULL;

	int oldMouseX = 0;
	int oldMouseY = 0;


	 int actionStartMouseX;
	 int actionStartMouseY;

	 bool grab = false;
	 bool grabMag = false;

	 int scrollTime = 0;


public:
	
	bool SetMapTipPos();

	int MoveGrab();

	int MagGrab(DammyWindow* dw, int maxWindow);
	
	void HitMapToPlayer(DammyWindow* dw, int maxWindow);

	int GameEnd(DammyWindow* dw, int maxWindow);

	int EndMove();
private:

	
};

enum ANIME_NAME {
	STAY,DASH,JUMP,DEATH,RESPAWN,CLEAR
};
class Player {
public:
	static Player* GetInstance() {
		static Player pl;
		return &pl;
	}
public:

	int moveXOld;
	int moveYOld;

	int x = 0;
	int y = 0;
	int xOld = NULL;
	int yOld = NULL;
	
	float speed = 0.1;
	float speedMax = 10;

	float size = 0.75;
	float oldSize = 0.75;

	int animeNumOld = 0;
	int animeNum = 0;
	int animeNumMax = 0;
	int* animeNumOnNum;

	int nowAnimePage = 0;
	int* animeNumOnNumChangeFrame;

	int animeF = 0;
	
	int windowNum=0;

	bool jump = true;
	float gravity = 1.0f;
	float gravityMax = 20;


	int vectorLR = 0;
	
	bool death = false;

	bool clear = false;

	bool key = false;

	bool switchButton = false;

	int stayCount = 0;

public:
	int Move(DammyWindow* dw,int maxWindow);

	int Anime();

	void Reset() {

		//int moveXOld;
		//int moveYOld;

		x = 0;
		y = 0;
		xOld = NULL;
		yOld = NULL;

		speed = 0.1;
		speedMax = 5;

		size = 0.75;

		animeNum = STAY;

		nowAnimePage = 0;

		animeF = 0;

		windowNum = 0;

		jump = true;
		gravity = 1.0f;
		gravityMax = 10;

		vectorLR = 0;

		death = false;

		clear = false;

		key = false;

		switchButton = false;

		stayCount = 0;
	}

};


class Cursor {
public:
	static Cursor* GetInstance() {
		static Cursor cr;
		return &cr;
	}
	bool anime = false;
	Pos2 pos;
	int animeF=0;
	const int animeMax = 110;
	int page = 0;
	void Anime() {
		if(animeF>=animeMax){
			animeF = 0;
			page = 0;
		}
		page = animeF / 10;
		animeF++;
	}
};