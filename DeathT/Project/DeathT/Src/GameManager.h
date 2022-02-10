#pragma once
#include "DirectX11Lib.h"
using namespace DXTK11LIB;

enum SCENE_NUM {
	TITLE,SELECT,OPTION,TOWER,TOWER_CLEAR, BATTLE, BATTLE_CLEAR,CLEDIT
};
class GameModel {
public:
	static GameModel* GetInstance() {
		static GameModel gm;
		return &gm;
	}
	FBX_MODEL* Player;
	FBX_MODEL* Player2;

	FBX_MODEL* Stage;
	FBX_MODEL* Block;
	FBX_MODEL* DangerBlock;
	
	FBX_MODEL* Enemy[3];

	FBX_MODEL* Item[2];

	FBX_MODEL* Boss1;
	FBX_MODEL* Boss2;
	FBX_MODEL* Boss3;

	//二人対戦用
	FBX_MODEL* playerAttackEffect_Red;
	FBX_MODEL* playerAttackEffect_Blue;
	FBX_MODEL* playerAttackEffect_RedRed;




	GameModel() {
		Player = new FBX_MODEL;
		Player2 = new FBX_MODEL;

		Stage = new FBX_MODEL;
		DangerBlock = new FBX_MODEL;
		Block = new FBX_MODEL;
		for (int i = 0; i < 3; i++) {
			Enemy[i] = new FBX_MODEL;
		}
		Item[0] = new FBX_MODEL;
		Item[1] = new FBX_MODEL;

		Boss1 = new FBX_MODEL;
		Boss2 = new FBX_MODEL;
		Boss3 = new FBX_MODEL;


		playerAttackEffect_Red = new FBX_MODEL;
		playerAttackEffect_RedRed = new FBX_MODEL;

		playerAttackEffect_Blue = new FBX_MODEL;

	}

};
class GameImage {
public:
	static GameImage* GetInstance() {
		static GameImage gi;
		return &gi;
	}
	Image logo;
	Image feedBlack; Image feedWhite;
	Image title;
	Image startStr;
	Image optionStr;
	Image endStr;

	
	Image gameBack;
	Image bar;
	Image barIcon;
	Image barIcon2;

	Image score;
	Image clearBack;
	Image bom;

	Image option;
	Image optionUI;
	Image optionCursor;
	Image optionBack;

	Image select;
	Image selectBattle;
	Image selectTower;
	Image selectTitle;
	Image selectOption;

	Image battle[3];
	Image hpBarBase;
	Image hpBar;

	Image stage1Clear;

	Image numbarFont[10];

	Image pause;
	Image pause_Back;
	Image pause_Select;
	Image pause_SelectConfirm;
	Image pause_SelectYes;
	Image pause_SelectNo;

	Image cleditBack;
	Image cleditPhoto[7];


};
class GameSound {
public:
	Sound_MP3 BGM[20];
	Sound_MP3 Enter;
	Sound_MP3 MovePlayer;
	Sound_MP3 SetBlock;
	Sound_MP3 Dead;
	Sound_MP3 MoveCursor;
	Sound_MP3 Attack;
	
	Sound_MP3 Boss1FrontAttack;
	Sound_MP3 Boss1Tackle;
	Sound_MP3 Boss1Avalanche;
	
	Sound_MP3 Boss2Laser;
	Sound_MP3 Boss2Homing;

	Sound_MP3 Boss3Attack;
	Sound_MP3 Boss3Laser;
	Sound_MP3 Boss3Thunder;


	Sound_MP3 EnemyDead;
	Sound_MP3 PlayerDamage;
	Sound_MP3 PlayerDead;
	Sound_MP3 ItemGet;
	Sound_MP3 Bom;

	static GameSound* GetInstance() {
		static GameSound gs;
		return &gs;
	}
};

class GameManager {
public:
	static GameManager* GetInstance() {
		static GameManager gm;
		return &gm;
	}
public:
	GameModel* gm;
	GameImage* gi;
	GameSound* gs;
	KeyInput* KEY;
	MouseInput* MOUSE;

	int scene = 0;
	int sceneBack = 0;

	int sceneOld = -1;
	int gameEnd = false;

	//オプションでいじる用
	float volumeMaster = 100;
	float volumeBGM = 50;
	float volumeSE = 50;
	//実際の数値
	float bgmVolume=0.5;
	float seVolume=0.5;

	bool credit = false;


	void AllLoad();

	void TitleLoad();
	void TitleUpdata();
	void TitleDraw();

	void OptionLoad();
	void OptionUpdata();
	void OptionDraw();

	void SelectLoad();
	void SelectUpdata();
	void SelectDraw();

	void GameLoad();
	void GameUpdata();
	void GameDraw();

	void ClearLoad();
	void ClearUpdata();
	void ClearDraw();

	void BattleLoad(int _stageNum);
	void BattleUpdata();
	void BattleDraw();

	void Battle_ClearSetScore(int _timeF,int _deleteEnemy);

	void Battle_ClearLoad();
	void Battle_ClearUpdata();
	void Battle_ClearDraw();

	void Battle_PlayerLoad();
	void Battle_PlayerUpdata();
	void Battle_PlayerDraw();

	void CleditLoad();
	void CleditUpdata();
	void CleditDraw();


};






