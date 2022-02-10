#include "GameManager.h"

namespace GameVariable {
	int goalHight = 10;

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
	class Player {
	public:
		Vector3 pos = Vector3(0.0f, 0.65f, 0.0f);
		Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 scale = Vector3(0.3, 0.3, 0.3);

		float speed = 0.655;
		int posNum_Fast = 5;
		int posNum = 5;

		int hight = 0;

		bool death = false;
		int deathCount = 0;
		int clearCount = 0;

		void Reset() {
			pos = Vector3(0.0f, 0.65f, 0.0f);
			degree = Vector3(0.0f, 180.0f, 180.0f);
			scale = Vector3(0.3, 0.3, 0.3);

			posNum_Fast = 5;
			posNum = 5;

			hight = 0;

			death = false;
			deathCount = 0;
			clearCount = 0;
		}
	};
	Player pl;
	StageHight stageHigh;
	class Effect {
	public:
		float speed;
		Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 scale = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 degree = Vector3(0.0f, 0.0f, 0.0f);
		void Set() {
			switch (pl.posNum_Fast) {
			case 1:
			case 2:
			case 3:
				pos.Z = -0.66;
				break;
			case 4:
			case 5:
			case 6:
				pos.Z = 0;
				break;
			case 7:
			case 8:
			case 9:
				pos.Z = +0.66;
				break;
			}
			switch (pl.posNum_Fast) {
			case 1:
			case 4:
			case 7:
				pos.X = -0.66;
				break;
			case 2:
			case 5:
			case 8:
				pos.X = 0;
				break;
			case 3:
			case 6:
			case 9:
				pos.X = +0.66;
				break;
			}
			//pos = pl.pos;
			pos.Y=pl.pos.Y;
			speed = (float)GetRand(0,9) / 100.0f;
			float randS = (float)GetRand(0,9) / 30.0f;
			float randD = (float)GetRand(0,360);
			scale.X = randS;
			scale.Y = randS;
			scale.Z = randS;
			degree.X = randD;
			degree.Y = randD;
			degree.Z = randD;

		};
		void Updata() {
			pos.Y += speed;
		};
	};
	Effect effect[10];

	class Block {
	public:
		bool set = false;
		int num = 0;
		float speed = 0.04f;
		Vector3 pos = Vector3(0.0f, 0.65f + 5, 0.0f);
		Block(int _num) {
			num = _num;
			//初期XZ位置設定
			switch (_num) {
			case 1:
				pos.X -= pl.speed;
				pos.Z += pl.speed;
				break;
			case 2:
				pos.Z += pl.speed;
				break;
			case 3:
				pos.X += pl.speed;
				pos.Z += pl.speed;
				break;

			case 4:
				pos.X -= pl.speed;
				break;

			case 6:
				pos.X += pl.speed;
				break;

			case 7:
				pos.Z -= pl.speed;
				pos.X -= pl.speed;
				break;
			case 8:
				
				pos.Z -= pl.speed;
				break;
			case 9:
				pos.X += pl.speed;
				pos.Z -= pl.speed;
				break;

			}
			//初期Y位置設定
			pos.Y += stageHigh.maxHight * 0.66;
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
			GameManager* gm = GameManager::GetInstance();
			//地面とブロックが接触していないとき

			if (pos.Y > 0.65f + (float)stageHigh.hightSet[num] * 0.66) {
				pos.Y -= speed;
			}
			else {
				if (set == false) {
					set = true;
					stageHigh.hightSet[num]++;
					gm->gs->SetBlock.Play();
				}
			}

			//プレイヤー死亡処理
			if (pos.Y >= 0.65f + stageHigh.hightSet[num] * 0.66f &&
				
				pos.Y - 0.66 <= 0.65f + stageHigh.hightSet[num] * 0.66f) {

				//XZ座標が落ちてくるブロックと同じ時
				if (num == pl.posNum_Fast && set == false) {
					if (pl.death == false) {
						pl.deathCount = 1;
						gm->gs->Dead.Play();
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

				int rand = GetRand(0,150) + 1;
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

	public:
		void SetList(int _left, int _right, int _up, int _down) {
			left = _left;
			right = _right;
			up = _up;
			down = _down;
		}
	};
	NumList numList[10];

	MODEL_DATA player_MD;
	MODEL_DATA stage_MD;

	float cameraTargetY = 6.0f;
	
	int moveNum = 0;

	int playerSpeed = 6;

}
using namespace GameVariable;


void GameManager::GameLoad() {

	moveNum = 0;
	player_MD.axisAngle = 0;

	stage_MD.pos = Vector3(0.0f, 0.0f, 0.0f);
	stage_MD.scale = Vector3(1, 0.3, 1);
	stage_MD.degree = Vector3(0.0f, 0.0f, 0.0f);

	cameraTargetY = 6.0f;
	//上下左右の移動できる位置の表示
	for (int i = 1; i < 10; i++) {
		int left = i - 1;
		if (left < 0 || left==3||left==6)
			left = 0;

		int right  = i + 1;
		if (right > 9 || right == 4 || right == 7)
			right = 0;

		int up = i - 3;
		if (up < 0)
			up = 0;

		int down = i + 3;
		if (down > 9)
			down = 0;
	
		numList[i].SetList(left,right,up,down);
	}

	pl.Reset();
	stageHigh.Reset();
	bs.Reset();


	player_MD.pos = pl.pos;
	player_MD.scale = pl.scale;
	player_MD.degree = pl.degree;

}
void GameManager::GameUpdata() {

	float r = GetHypotenuse(0.325, 0.325);
	int leftHeight = stageHigh.hightSet[numList[pl.posNum].left];
	int rightHeight = stageHigh.hightSet[numList[pl.posNum].right];
	int upHeight = stageHigh.hightSet[numList[pl.posNum].up];
	int downHeight = stageHigh.hightSet[numList[pl.posNum].down];

	int moveHeight = stageHigh.hightSet[pl.posNum];
	

	if (moveNum == 0) {
		if (KEY->key[DIK_A] == 1&&
			leftHeight - moveHeight<2&& leftHeight - moveHeight >-2&& numList[pl.posNum].left!=0) {
			pl.posNum_Fast = numList[pl.posNum_Fast].left;
			moveNum = DIK_A;		
			player_MD.axis = Vector3(0, 0, 1);//回転軸設定 Z
			player_MD.axisR = Vector3(r, 0, 0);//半径設定
			if (leftHeight - moveHeight == 1) {
				player_MD.axisAngle = 45 - (leftHeight - moveHeight) * 90;
			}
			else {
				player_MD.axisAngle = 45;
			}
			player_MD.degree.Z += 45;

			player_MD.pos.X = pl.pos.X - 0.33;
			if (leftHeight - moveHeight < 1) {
				player_MD.pos.Y = pl.pos.Y - 0.33;
			}
			else {
				player_MD.pos.Y = pl.pos.Y - 0.33 + 0.66;
			}
		}
		if (KEY->key[DIK_D] == 1 &&
			rightHeight - moveHeight<2 && rightHeight - moveHeight >-2 && numList[pl.posNum].right != 0) {
			pl.posNum_Fast = numList[pl.posNum_Fast].right;
			moveNum = DIK_D;
			player_MD.axis = Vector3(0, 0, 1);//回転軸設定 Z
			player_MD.axisR = Vector3(r, 0, 0);//半径設定
			if (rightHeight - moveHeight == 1) {
				player_MD.axisAngle = 135 + (rightHeight - moveHeight) * 90;
			}
			else {
				player_MD.axisAngle = 135;
			}
			
			player_MD.degree.Z += 135;

			player_MD.pos.X = pl.pos.X + 0.33;
			if (rightHeight - moveHeight < 1) {
				player_MD.pos.Y = pl.pos.Y - 0.33;
			}
			else {
				player_MD.pos.Y = pl.pos.Y - 0.33 + 0.66;
			}
		}
		if (KEY->key[DIK_W] == 1 &&
			upHeight - moveHeight<2 && upHeight - moveHeight >-2 && numList[pl.posNum].up != 0) {
			moveNum = DIK_W;
			pl.posNum_Fast = numList[pl.posNum_Fast].up;
			player_MD.axis = Vector3(1, 0, 0);//回転軸設定 X
			player_MD.axisR = Vector3(0, 0, r);//半径設定

			if (upHeight - moveHeight == 1) {
				player_MD.axisAngle = -135 - 90;
			}
			else {
				player_MD.axisAngle = -135;
			}

			
			player_MD.degree.X -= 45;

			player_MD.pos.Z = pl.pos.Z + 0.33;
			if (upHeight - moveHeight < 1) {
				player_MD.pos.Y = pl.pos.Y - 0.33;
			}
			else {
				player_MD.pos.Y = pl.pos.Y - 0.33 + 0.66;
			}
		}
		if (KEY->key[DIK_S] == 1 &&
			downHeight - moveHeight<2 && downHeight - moveHeight >-2 && numList[pl.posNum].down != 0) {
			pl.posNum_Fast = numList[pl.posNum_Fast].down;
			moveNum = DIK_S;
			player_MD.axis = Vector3(1, 0, 0);//回転軸設定 Z
			player_MD.axisR = Vector3(0, 0, r);//半径設定


			if (downHeight - moveHeight == 1) {
				player_MD.axisAngle = -45 + 90;
			}
			else {
				player_MD.axisAngle = -45;
			}
			player_MD.degree.X -= 45;

			player_MD.pos.Z = pl.pos.Z - 0.33;
			if (downHeight - moveHeight < 1) {
				player_MD.pos.Y = pl.pos.Y - 0.33;
			}
			else {
				player_MD.pos.Y = pl.pos.Y - 0.33 + 0.66;
			}
		}
	}
	else {
		
		int dist = 0;
		switch (moveNum) {
		case DIK_A:

			switch (leftHeight - moveHeight) {
			case 1:
			
				break;
			case -1:
				dist = 90;
				break;
			}

			player_MD.axisAngle += playerSpeed;
			if (player_MD.axisAngle >= 135+dist) {
				moveNum = 0;
				pl.pos.X -= pl.speed;
				pl.pos.Y += pl.speed* (leftHeight - moveHeight);

				player_MD.pos = pl.pos;
				//player_MD.degree.Z += 45-dist;
				player_MD.degree.Z += -135 - dist;

				player_MD.axisAngle = 0;
				player_MD.axisR = Vector3(0, 0, 0);//半径設定

				switch (leftHeight - moveHeight) {
				case 1:
					pl.hight++;
					break;
				case -1:
					pl.hight--;
					break;
				}
				pl.posNum = numList[pl.posNum].left;
				
				gs->MovePlayer.Play();
			}
			break;
		case DIK_D:
			switch (rightHeight - moveHeight) {
			case 1:
				
				break;
			case -1:
				dist = 90;
				break;
			}
			player_MD.axisAngle -= playerSpeed;
			if (player_MD.axisAngle <= 45-dist) {
				moveNum = 0;
				pl.pos.X += pl.speed;
				pl.pos.Y += pl.speed * (rightHeight - moveHeight);

				player_MD.pos = pl.pos;
				player_MD.degree.Z += -45+dist;

				player_MD.axisAngle = 0;
				player_MD.axisR = Vector3(0, 0, 0);//半径設定

				switch (rightHeight - moveHeight) {
				case 1:
					pl.hight++;
					break;
				case -1:
					pl.hight--;
					break;
				}

				pl.posNum = numList[pl.posNum].right;
			}
			break;
		case DIK_W:
			switch (upHeight - moveHeight) {
			case -1:
				dist = 90;
				break;
			}
			player_MD.axisAngle+= playerSpeed;
			if (player_MD.axisAngle >= -45+dist) {
				moveNum = 0;
				pl.pos.Z += pl.speed;
				pl.pos.Y += pl.speed * (upHeight - moveHeight);

				player_MD.pos = pl.pos;
				player_MD.degree.X += 45;

				player_MD.axisAngle = 0;
				player_MD.axisR = Vector3(0, 0, 0);//半径設定

				switch (upHeight - moveHeight) {
				case 1:
					pl.hight++;
					break;
				case -1:
					pl.hight--;
					break;
				}

				pl.posNum = numList[pl.posNum].up;
			}
			break;
		case DIK_S:
			switch (downHeight - moveHeight) {
			case -1:
				dist = 90;
				break;
			}
			player_MD.axisAngle-= playerSpeed;
			if (player_MD.axisAngle <= -135-dist) {
				moveNum = 0;
				pl.pos.Z -= pl.speed;
				pl.pos.Y += pl.speed * (downHeight - moveHeight);

				player_MD.pos = pl.pos;
				player_MD.degree.X += 45;

				player_MD.axisAngle = 0;
				player_MD.axisR = Vector3(0, 0, 0);//半径設定

				switch (downHeight - moveHeight) {
				case 1:
					pl.hight++;
					break;
				case -1:
					pl.hight--;
					break;
				}

				pl.posNum = numList[pl.posNum].down;
			}
			break;

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
					gs->BGM[scene].Stop();
					scene = SELECT;
					//TODO
					//ResetALL();
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
					gs->BGM[scene].Stop();
					scene = TOWER_CLEAR;
				}
			}

}


void GameManager::GameDraw() {
	bool quarterView = true;
	DirectGraphics::GetInstance()->SetLight(Vector3(quarterView * 3, cameraTargetY, 1), Vector3(0, cameraTargetY - 5, 0), Vector3(0, 1, 0), 45, Vector3(1, 1, 1), true);
	//カメラ制御

	float tmpdist = 0.05f;
	float cameraSpeed = 0.01f;
	if (cameraTargetY + tmpdist < 6 + pl.hight * 0.66) {
		cameraTargetY += cameraSpeed;
	}
	else if (cameraTargetY - tmpdist > 6 + pl.hight * 0.66) {
		cameraTargetY -= cameraSpeed;
	}


	for (int i = 0; i < bs.blockList.size(); i++) {
		MODEL_DATA md;
		md.pos = bs.blockList[i].pos;
		md.scale = Vector3(0.33, 0.33, 0.33);
		md.degree = Vector3(0.0f, 0.0f, 0.0f);
		gm->Block->RenderShadowMap(&md);

	}

	//プレイヤーの描画
	if (pl.death == false) {
		gm->Player->RenderShadowMap(&player_MD);
	}
	gm->Stage->RenderShadowMap(&stage_MD);
	MODEL_DATA mdS;
	mdS.degree.X = -90;
	mdS.degree.Y = 180;

	DirectGraphics::GetInstance()->SetCamera(Vector3(quarterView * 3, cameraTargetY, -3), Vector3(0, cameraTargetY - 5, 0), Vector3(0, 1, 0));
	DirectGraphics::GetInstance()->SetProjection(45.0f, 0.1f, 1000.0f, true);
	


	//ゲームの背景
	int gameBackX = 960 - bs.countGameAll % 1920;
	int gameBackY = 540 + bs.countGameAll % 1080;
	gi->gameBack.Draw(gameBackX, gameBackY - 1080);
	gi->gameBack.Draw(gameBackX, gameBackY);
	gi->gameBack.Draw(gameBackX + 1920, gameBackY);
	gi->gameBack.Draw(gameBackX + 1920, gameBackY - 1080);
	//上昇バーの表示
	gi->bar.Draw(1750, 540);
	gi->barIcon.Draw(1750, 900 - pl.hight * (900 / goalHight));

	gm->Stage->RenderShadowModel(&stage_MD);
	//プレイヤーの描画
	if (pl.death == false) {


		gm->Player->RenderShadowModel(&player_MD);
	}


	

	for (int i = 0; i < bs.blockList.size(); i++) {
		MODEL_DATA md;
		md.pos = bs.blockList[i].pos;
		md.scale = Vector3(0.33, 0.33, 0.33);
		md.degree = Vector3(0.0f, 0.0f, 0.0f);
		gm->Block->RenderShadowModel(&md);
	}

	//死亡エフェクト
	if (pl.deathCount) {
		for (int i = 0; i < 10; i++) {
			MODEL_DATA md;
			md.pos = effect[i].pos;
			md.degree = effect[i].degree;
			md.scale = effect[i].scale;
			gm->Player->RenderShadowModel(&md);
		}
		gi->feedBlack.Draw(960, 540, 0, 1, (float)pl.deathCount / 100.0f);
	}
	//クリアエフェクト
	if (pl.clearCount) {
		gi->feedWhite.Draw(960, 540, 0, 1, (float)pl.clearCount / 100.0f);
	}
}

