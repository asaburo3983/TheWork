#define _CRT_SECURE_NO_WARNINGS
#include "MainGame.h"
class Player {
public:
	Vector3 pos = Vector3(0.0f, -0.65f, 0.0f);
	Vector3 degree = Vector3(90.0f, 225.0f, 0.0f);
	Vector3 scale = Vector3(0.3, 0.3, 0.3);

	float speed = GetHypotenuse(0.33, 0.33) * 2;

	int posNum = 5;

	int hight = 0;

	bool death = false;
	int deathCount = 0;
	int clearCount = 0;

	void Reset() {
		pos = Vector3(0.0f, -0.65f, 0.0f);
		degree = Vector3(90.0f, 225.0f, 0.0f);
		scale = Vector3(0.3, 0.3, 0.3);

		posNum = 5;

		hight = 0;

		death = false;
		deathCount = 0;
		clearCount = 0;
	}
};
Player pl;
Camera cam;
Light lig;

Sound_MP3 BGM[3];
Sound_MP3 Enter;
Sound_MP3 MovePlayer;
Sound_MP3 SetBlock;
Sound_MP3 Dead;


class StageHight {
public:
	int hightSet[10];

	int AllHightSet = 0;

	int maxHight = 0;
	int hight[10];//その場限りの高さ、高さ０の所だけブロック配置する

	void Reset() {

		AllHightSet = 0;
		maxHight = 0;
		for (int i = 0; i < 10; i++) {
			hightSet[i] = 0;
			hight[i] = 0;
		}
	}
};
StageHight stageHigh;
class Effect {
public:
	float speed;
	Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 scale = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
	void Set() {
		pos = pl.pos;
		speed = (float)GetRand(1,10) / 100.0f;
		float randS = (float)GetRand(1,10) / 30.0f;
		float randD = (float)GetRand(1,360);
		scale.X = randS;
		scale.Y = randS;
		scale.Z = randS;
		degree.X = randD;
		degree.Y = randD;
		degree.Z = randD;

	};
	void Updata() {
		pos.Y -= speed;
	};
};
Effect effect[10];

class Block {
public:
	bool set = false;
	int num = 0;
	float speed = 0.04f;
	Vector3 pos = Vector3(0.0f, -0.65f - 5, 0.0f);
	Block(int _num) {
		num = _num;
		//初期XZ位置設定
		switch (_num) {
		case 1:
			pos.Z += pl.speed;
			break;
		case 2:
			pos.X += pl.speed / 2;
			pos.Z -= pl.speed / 2;
			break;
		case 3:
			pos.X -= pl.speed / 2;
			pos.Z -= pl.speed / 2;
			break;
		case 4:
			pos.X += pl.speed;
			break;
		case 6:
			pos.X -= pl.speed;
			break;
		case 7:
			pos.Z -= pl.speed;
			break;
		case 8:
			pos.X += pl.speed / 2;
			pos.Z += pl.speed / 2;
			break;
		case 9:
			pos.X -= pl.speed / 2;
			pos.Z += pl.speed / 2;
			break;

		}
		//初期Y位置設定
		pos.Y -= stageHigh.maxHight * 0.66;
		stageHigh.hight[_num]++;

		//全段１段下げる処理
		for (int i = 1; i < 10; i++) {
			if (stageHigh.maxHight < stageHigh.hight[i]) {
				stageHigh.maxHight = stageHigh.hight[i];
			}
		}

	}

	void Updata() {
		//落とす処理

		if (pos.Y < -0.65f - (float)stageHigh.hightSet[num] * 0.66) {
			pos.Y += speed;
		}
		else {
			if (set == false) {
				set = true;
				stageHigh.hightSet[num]++;
				SetBlock.Play();
			}
		}

		//プレイヤー死亡処理
		if (pos.Y <= -0.65f - (float)stageHigh.hightSet[num] * 0.66f && pos.Y + 0.66 >= -0.65f - (float)stageHigh.hightSet[num] * 0.66f) {
			if (num == pl.posNum && set == false) {
				if (pl.death == false) {
					pl.deathCount = 1;
					Dead.Play();
					for (int i = 0; i < 10; i++) {
						effect[i].Set();
					}
				}
				pl.death = true;

			}
		}
	}
};
class BlockSystem {
public:

	int countGameAll = 0;
	int count = 0;
	int countMax = 100;

	std::vector<Block> blockList;
	void Updata() {
		countGameAll++;
		if (countGameAll % 900 == 0) {
			countMax -= 10;
			if (countMax < 60) {
				countMax = 60;
			}
		}

		if (count < countMax) {
			count++;
		}
		else {
			//ランダムで落ちブロック生成
			count = 0;

			int rand = GetRand(0,149) + 1;
			if (rand <= 89) {
				rand /= 10;
				rand += 1;
			}
			else {
				int min = 200;
				int minNum = 1;
				for (int i = 1; i < 10; i++) {
					if (stageHigh.hight[i] < min) {
						minNum = i;
						min = stageHigh.hight[i];
					}
					else if (stageHigh.hight[i] == min) {
						if (GetRand(0,1)) {
							minNum = i;
						}
					}
				}
				rand = minNum;
			}
			if (rand >= 10 || rand <= 0) {
				rand = GetRand(0,8) + 1;
			}
			Block block(rand);
			blockList.push_back(block);
		}

		for (int i = 0; i < blockList.size(); i++) {
			blockList[i].Updata();
		}

	}

	void Reset() {
		countGameAll = 0;
		count = 0;
		countMax = 100;
		while (blockList.size()) {
			blockList.pop_back();
		}
	}
};
BlockSystem bs;
class NumList {
public:
	int left;
	int right;
	int up;
	int down;

	bool LD = false;
	bool LU = false;
	bool RD = false;
	bool RU = false;

	bool UR = false;
	bool UL = false;
	bool DR = false;
	bool DL = false;

public:
	void SetList(int _left, int _right, int _up, int _down) {
		left = _left;
		right = _right;
		up = _up;
		down = _down;
	}
};
NumList numList[10];

const int goalHight = 10;
int scene = 0;
int titleCount = 0;
int feedCount = 0;
Image title;
Image feedBlack; Image feedWhite;
Image gameBack;
Image bar;
Image barIcon;
Image score;
Image clearBack;

void ResetALL() {
	pl.Reset();
	stageHigh.Reset();
	bs.Reset();

	scene = 0;
	titleCount = 0;
	feedCount = 0;
}
void MainGameLoad() {
	//カメラの初期設定
	cam.pos.p[0] = 0;
	cam.pos.p[1] = -4;
	cam.pos.p[2] = -4;
	//ライトの初期設定
	lig.pos.p[0] = 0;
	lig.pos.p[1] = -5;
	lig.pos.p[2] = 0;
	//上下左右の移動できる位置の表示
	numList[1].SetList(8, 9, 0, 5);
	numList[1].LD = true; numList[1].RD = true;

	numList[2].SetList(4, 3, 8, 7);
	numList[2].LU = true; numList[2].DR = true;

	numList[3].SetList(2, 6, 9, 7);
	numList[3].RU = true; numList[3].DL = true;

	numList[4].SetList(0, 5, 8, 2);
	numList[4].UR = true; numList[4].DR = true;

	numList[5].SetList(4, 6, 1, 7);
	numList[6].SetList(5, 0, 9, 3);
	numList[6].UL = true; numList[6].DL = true;

	numList[7].SetList(2, 3, 5, 0);
	numList[7].LU = true; numList[7].RU = true;

	numList[8].SetList(4, 9, 1, 2);
	numList[8].UR = true; numList[8].LD = true;

	numList[9].SetList(8, 6, 1, 3);
	numList[9].UL = true; numList[9].RD = true;

	GameModel* gm = GameModel::GetInstance();
	//モデルデータのロード
	gm->Player = new FbxMeshFile();
	gm->Player->con = DirectGraphics::GetInstance()->GetContext();
	gm->Player->Load("Res/Model/Player.fbx", "Data/Shader/Vertex_NoAnimeShadowOn.cso", "Geometry_NoAnimeShadowOn.cso", "Data/Shader/Pixel_NoShadow.cso");

	gm->Block = new FbxMeshFile();
	gm->Block->con = DirectGraphics::GetInstance()->GetContext();
	gm->Block->Load("Res/Model/Block.fbx", "Data/Shader/Vertex_NoAnimeShadowOn.cso", "Geometry_NoAnimeShadowOn.cso", "Data/Shader/Pixel_NoShadow.cso");

	gm->Stage = new FbxMeshFile();
	gm->Stage->con = DirectGraphics::GetInstance()->GetContext();
	gm->Stage->Load("Res/Model/Stage.fbx", "Data/Shader/Vertex_NoAnimeShadowOn.cso", "Geometry_NoAnimeShadowOn.cso", "Data/Shader/Pixel_NoShadow.cso");

	//画像、音楽のロード
	title.Load("Data/Image/title.png");
	feedBlack.Load("Data/Image/feedBlack.png");
	feedWhite.Load("Data/Image/feedWhite.png");

	gameBack.Load("Data/Image/gameBack.png");
	bar.Load("Data/Image/bar.png");
	barIcon.Load("Data/Image/barIcon.png");

	score.Load("Data/Image/Score.png");

	BGM[0].Load("Data/Sound/Title.mp3");
	BGM[1].Load("Data/Sound/Game.mp3");
	BGM[2].Load("Data/Sound/Clear.mp3");

	MovePlayer.Load("Data/Sound/MoveSE.mp3");
	SetBlock.Load("Data/Sound/SetSE.mp3");
	Enter.Load("Data/Sound/EnterSE.mp3");
	Dead.Load("Data/Sound/DeadSE.mp3");

	clearBack.Load("Data/Image/clearBack.png");


}

void MainGameUpdata() {
	GameStatic* gs = GameStatic::GetInstance();
	gs->Updata();

	BGM[scene].Play();

	if (scene == 0) {
		if (titleCount < 360) {
			titleCount++;
		}
		else {
			titleCount = 0;
		}
		DirectGraphics::GetInstance()->SetLight(Vector3(0, -4, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
		DirectGraphics::GetInstance()->SetCamera(Vector3(0, -2, -4), Vector3(0, -1, 0), Vector3(0, 0, 0.1));
		DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

		if (feedCount == 0 && (gs->KEY->key[DIK_A] == 1 ||
			gs->KEY->key[DIK_W] == 1 ||
			gs->KEY->key[DIK_S] == 1 ||
			gs->KEY->key[DIK_D] == 1)) {
			feedCount = 1;
			Enter.Play();
		}
		if (feedCount) {
			feedCount++;
			if (feedCount >= 100) {
				feedCount = 0;
				BGM[scene].Stop();
				scene = 1;
			}
		}
	}
	else if (scene == 1) {
		DirectGraphics::GetInstance()->SetLight(Vector3(0, lig.pos.p[1], 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
		DirectGraphics::GetInstance()->SetCamera(Vector3(cam.pos.p[0], cam.pos.p[1] - pl.hight * 0.66, cam.pos.p[2]), Vector3(0, -1 - pl.hight * 0.66, 0), Vector3(0, 0, 0.1));
		DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

		if (gs->KEY->key[DIK_A] == 1) {

			//１段差以内の場合
			if (stageHigh.hightSet[numList[pl.posNum].left] >= stageHigh.hightSet[pl.posNum] - 1
				&& stageHigh.hightSet[numList[pl.posNum].left] <= stageHigh.hightSet[pl.posNum] + 1) {

				if (numList[pl.posNum].left) {
					if (numList[pl.posNum].LU) {
						pl.pos.X += pl.speed / 2;
						pl.pos.Z += pl.speed / 2;
					}
					else if (numList[pl.posNum].LD) {
						pl.pos.X += pl.speed / 2;
						pl.pos.Z -= pl.speed / 2;
					}
					else {
						pl.pos.X += pl.speed;
					}


					if (stageHigh.hightSet[numList[pl.posNum].left] > stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y -= 0.66f;
						pl.hight++;
					}
					else if (stageHigh.hightSet[numList[pl.posNum].left] < stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y += 0.66f;
						pl.hight--;
					}
					pl.posNum = numList[pl.posNum].left;

					MovePlayer.Play();
				}
			}

		}
		if (gs->KEY->key[DIK_D] == 1) {
			if (stageHigh.hightSet[numList[pl.posNum].right] >= stageHigh.hightSet[pl.posNum] - 1
				&& stageHigh.hightSet[numList[pl.posNum].right] <= stageHigh.hightSet[pl.posNum] + 1) {

				if (numList[pl.posNum].right) {

					if (numList[pl.posNum].RU) {
						pl.pos.X -= pl.speed / 2;
						pl.pos.Z += pl.speed / 2;
					}
					else if (numList[pl.posNum].RD) {
						pl.pos.X -= pl.speed / 2;
						pl.pos.Z -= pl.speed / 2;
					}
					else {
						pl.pos.X -= pl.speed;
					}

					if (stageHigh.hightSet[numList[pl.posNum].right] > stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y -= 0.66f;
						pl.hight++;
					}
					else if (stageHigh.hightSet[numList[pl.posNum].right] < stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y += 0.66f;
						pl.hight--;
					}
					pl.posNum = numList[pl.posNum].right;
					MovePlayer.Play();
				}
			}
		}
		if (gs->KEY->key[DIK_W] == 1) {
			if (stageHigh.hightSet[numList[pl.posNum].up] >= stageHigh.hightSet[pl.posNum] - 1
				&& stageHigh.hightSet[numList[pl.posNum].up] <= stageHigh.hightSet[pl.posNum] + 1) {

				if (numList[pl.posNum].up) {
					MovePlayer.Play();
					if (numList[pl.posNum].UR) {
						pl.pos.X -= pl.speed / 2;
						pl.pos.Z += pl.speed / 2;
					}
					else if (numList[pl.posNum].UL) {
						pl.pos.Z += pl.speed / 2;
						pl.pos.X += pl.speed / 2;

					}
					else {
						pl.pos.Z += pl.speed;
					}
					if (stageHigh.hightSet[numList[pl.posNum].up] > stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y -= 0.66f;
						pl.hight++;
					}
					else if (stageHigh.hightSet[numList[pl.posNum].up] < stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y += 0.66f;
						pl.hight--;
					}
					pl.posNum = numList[pl.posNum].up;
				}
			}

		}
		if (gs->KEY->key[DIK_S] == 1) {
			if (stageHigh.hightSet[numList[pl.posNum].down] >= stageHigh.hightSet[pl.posNum] - 1
				&& stageHigh.hightSet[numList[pl.posNum].down] <= stageHigh.hightSet[pl.posNum] + 1) {

				if (numList[pl.posNum].down) {
					MovePlayer.Play();
					if (numList[pl.posNum].DR) {
						pl.pos.X -= pl.speed / 2;
						pl.pos.Z -= pl.speed / 2;
					}
					else if (numList[pl.posNum].DL) {
						pl.pos.Z -= pl.speed / 2;
						pl.pos.X += pl.speed / 2;

					}
					else {
						pl.pos.Z -= pl.speed;
					}
					if (stageHigh.hightSet[numList[pl.posNum].down] > stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y -= 0.66f;
						pl.hight++;
					}
					else if (stageHigh.hightSet[numList[pl.posNum].down] < stageHigh.hightSet[pl.posNum]) {
						pl.pos.Y += 0.66f;
						pl.hight--;
					}
					pl.posNum = numList[pl.posNum].down;
				}
			}
		}

		bs.Updata();

		//死亡処理
		if (pl.deathCount) {
			pl.deathCount++;
			for (int i = 0; i < 10; i++) {
				effect[i].Updata();
			}
			if (pl.deathCount >= 100) {
				pl.deathCount = 0;
				BGM[scene].Stop();
				scene = 0;
				ResetALL();
			}
		}
		if (pl.hight >= goalHight) {
			if (pl.clearCount == 0)
				pl.clearCount++;
		}
		if (pl.clearCount) {
			pl.clearCount++;
			if (pl.clearCount >= 100) {
				pl.clearCount = 0;
				BGM[scene].Stop();
				scene = 2;
			}
		}
	}
	else if (scene == 2) {
		if (titleCount < 360) {
			titleCount++;
		}
		else {
			titleCount = 0;
		}
		if (feedCount == 0 && (gs->KEY->key[DIK_A] == 1 ||
			gs->KEY->key[DIK_W] == 1 ||
			gs->KEY->key[DIK_S] == 1 ||
			gs->KEY->key[DIK_D] == 1)) {
			feedCount = 1;
			Enter.Play();
		}
		if (feedCount) {
			feedCount++;
			if (feedCount >= 100) {
				feedCount = 0;
				BGM[scene].Stop();
				scene = 0;
				ResetALL();
			}
		}
	}
}
void MainGameDraw() {

	GameModel* gm = GameModel::GetInstance();


	if (scene == 0) {
		title.Draw(960, 540);
		gm->Player->Render(Vector3(-1.5f, -0.2f, 0.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(90.0f, 225.0f + titleCount, 0.0f));

		gm->Block->Render(Vector3(-0.1, -4.0f + (float)titleCount / 50.0, 0.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(90.0f, 45.0f + titleCount, 0.0f));
		gm->Block->Render(Vector3(-2.5, -4.5f + (float)titleCount / 50.0, 1.0f), Vector3(0.5f, 0.5f, 0.5f), Vector3(90.0f, 180.0f + titleCount, 0.0f));

		feedBlack.Draw(960, 540, 0, 1, (float)feedCount / 100.0f);
	}
	else if (scene == 1) {
		bs.countGameAll;
		int gameBackX = 960 - bs.countGameAll % 1920;
		int gameBackY = 540 + bs.countGameAll % 1080;

		gameBack.Draw(gameBackX, gameBackY - 1080);
		gameBack.Draw(gameBackX, gameBackY);
		gameBack.Draw(gameBackX + 1920, gameBackY);
		gameBack.Draw(gameBackX + 1920, gameBackY - 1080);

		bar.Draw(1750, 540);
		barIcon.Draw(1750, 900 - pl.hight * (900 / goalHight));

		if (pl.death == false) {
			gm->Player->Render(pl.pos, pl.scale, pl.degree);
		}

		Vector3 stagePos = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 stageScale = Vector3(1, 0.3, 1);
		Vector3 stageDegree = Vector3(0.0f, 45.0f, 0.0f);
		gm->Stage->Render(stagePos, stageScale, stageDegree);

		for (int i = 0; i < bs.blockList.size(); i++) {
			gm->Block->Render(bs.blockList[i].pos, Vector3(0.33, 0.33, 0.33), Vector3(0.0f, 45.0f, 0.0f));
		}

		//死亡エフェクト
		if (pl.deathCount) {
			for (int i = 0; i < 10; i++) {
				gm->Player->Render(effect[i].pos, effect[i].scale, effect[i].degree);
			}
			feedBlack.Draw(960, 540, 0, 1, (float)pl.deathCount / 100.0f);
		}
		//クリアエフェクト
		if (pl.clearCount) {
			feedWhite.Draw(960, 540, 0, 1, (float)pl.clearCount / 100.0f);
		}
	}
	else if (scene == 2) {
		clearBack.Draw(960, 540);
		DirectGraphics::GetInstance()->SetLight(Vector3(0, -4, 0), Vector3(0, 0, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
		DirectGraphics::GetInstance()->SetCamera(Vector3(0, -2, -4), Vector3(0, -1, 0), Vector3(0, 0, 0.1));
		DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);

		gm->Player->Render(Vector3(0.0f, -0.2f, 0.0f), Vector3(0.7f, 0.7f, 0.7f), Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount));
		gm->Player->Render(Vector3(-1.7f, -0.2f, 0.0f), Vector3(0.4f, 0.4f, 0.4f), Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount));
		gm->Player->Render(Vector3(+1.7f, -0.2f, 0.0f), Vector3(0.4f, 0.4f, 0.4f), Vector3(90.0f + titleCount, 225.0f + titleCount, 0.0f + titleCount));

		feedBlack.Draw(960, 540, 0, 1, (float)feedCount / 100.0f);

	}
}