#include "BossSystem.h"

void Boss1::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 0, 3);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 5;
	HP_MAX = 3000;//初期HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//攻撃間隔
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss1::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//一定時間おきにプレイヤーの位置を見て行動を決める
		if (animeNum == 3 || animeNum == 7 || animeNum == 8) {
			attackCount++;
		}
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;
			bool tackleAttack = false;
			bool frontAttack = false;
			bool waveAttack = false;

			if (player->x < 6 && player->x>2 && player->y > 4) {
				tackleAttack = true;
			}
			else if (player->x < 6 && player->x>2 && player->y <= 4) {
				frontAttack = true;
			}

			int parsent = GetRand(1, 100);
			if (frontAttack) {
				animeNum = 1;

			}
			else if (tackleAttack) {
				animeNum = 4;

			}
			else {
				animeNum = 5;
			}
			if (parsent <= 5) {
				animeNum = 5;
			}

			if (animeNum == 5) {

			}
		}

		//タックルの座標戻し
		if (animeNum != 4) {
			if (md.pos.Z < 3) {
				md.pos.Z += 0.2f;
				if (md.pos.Z > 3) {
					md.pos.Z = 3;
				}
			}
			else if (md.pos.Z > 3) {
				md.pos.Z -= 0.2f;
				if (md.pos.Z < 3) {
					md.pos.Z = 3;
				}
			}

		}
		switch (animeNum) {
		case 3://STAY
			break;
		case 1://FRONT_ATTACK
			animeCount++;
			//プレイヤーへのダメージ処理
			if (animeCount == animeConstFrame[0] / 2) {
				gm->gs->Boss1FrontAttack.Play(false);
				if (player->x <= 6 && player->x >= 2 && player->y <= 4&&player->damageCount==0) {
					player->damage = true;
				}
			}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[0]) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		case 4://TACKLE_ATTACK
			if (animeCount == 60) {
				gm->gs->Boss1Tackle.Play(false);
			}
			if (animeCount > 60) {
				md.pos.Z -= tackleSpeed;

			}
			animeCount++;
			//プレイヤーへのダメージ処理
			Pos3 center;
			center.p[0] = player->md.pos.X;
			center.p[1] = player->md.pos.Y;
			center.p[2] = player->md.pos.Z;
			Pos3 point;
			point.p[0] = md.pos.X;
			point.p[1] = md.pos.Y;
			point.p[2] = md.pos.Z;
			Pos3 size;
			size.p[0] = 3.3;
			size.p[1] = 3.3;
			size.p[2] = 0.33;

			if (HitRect3D(center, size, point) && player->damageCount == 0) {
				player->damage = true;
			}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[1]) {
				animeCount = 0;
				animeNum = 3;
				md.pos.Z = 10;
			}
			break;
		case 5://WAVE_ATTACK
			animeCount++;
			if (animeCount == 100) {
				gm->gs->Boss1Avalanche.Play(false);
				Pos2 bossPos;
				bossPos.p[0] = md.pos.X;
				bossPos.p[1] = md.pos.Z;

				int tmp = GetRand(0, 1);
				for (int i = 0; i < 10; i++) {

					int randX = GetRand(0, 8);
					int randZ = GetRand(0, 8);

					Boss1Wave b1w(0.06f, randX, randZ, md.pos.Z);
					boss1Wave.push_back(b1w);

				}
			}
			if (animeCount >= animeConstFrame[2]) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		case 7://ダメージを受けた時
		case 8:
			animeCount++;
			if (animeCount >= 40) {
				animeCount = 0;
				animeNum = 3;
			}
			break;
		}

		//プレイヤー攻撃エフェクトとの当たり判定
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;
					point.p[1] = md.pos.Y;
					point.p[2] = md.pos.Z;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);
						if (animeNum == 3) {

							animeNum = 7 + GetRand(0, 1);
						}
						break;

					}
				}
			}
		}
		//無敵時間
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//ウェーブ攻撃更新
		for (int i = 0; i < boss1Wave.size(); i++)
		{
			boss1Wave[i].Updata();
			if (boss1Wave[i].r > 10) {
				Boss1WaveDeathEffect b1wde;
				b1wde.md = boss1Wave[i].md;

				Pos3 center;
				center.p[0] = player->md.pos.X;
				center.p[1] = player->md.pos.Y;
				center.p[2] = player->md.pos.Z;
				Pos3 point;
				point.p[0] = b1wde.md.pos.X;
				point.p[1] = b1wde.md.pos.Y;
				point.p[2] = b1wde.md.pos.Z;
				Pos3 size;
				size.p[0] = 0.66;
				size.p[1] = 0.66;
				size.p[2] = 0.66;
				if (HitRect3D(center, size, point)) {
					player->damage = true;
				}
				//ウェーブ壊れエフェクト追加
				boss1WaveDeathEffect.push_back(b1wde);
				//ウェーブ削除
				boss1Wave.erase(boss1Wave.begin() + i);

			}
		}
		//ウェーブ壊れエフェクト更新
		for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
			boss1WaveDeathEffect[i].Updata();
			if (boss1WaveDeathEffect[i].count > boss1WaveDeathEffect[i].countMax) {
				//ウェーブ壊れエフェクト削除
				boss1WaveDeathEffect.erase(boss1WaveDeathEffect.begin() + i);
			}
		}
		//死亡処理
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//死亡処理（アニメーションストップ）
		deathCount++;
		if (deathCount > 60) {
			deathCount = 60;
		}
	}


	
	//ステージ番号操作
	gameSystem->stageNum = 1;
	//BOSS1の出現時のカメラの操作
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 6;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//道中BGMを止める
	}
	else {

		//出現時アニメーション
		if (gameSystem->cameraBossCount == 20) {
			animeNum = 3;
		}
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}
	//ボス死亡時に次の面へ移動する
	if (animeNum == 0) {
		//消えていく
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//完全に消えた時ボスの処理を停止する
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss1Exist = false;//完全なボスの停止
				enemySystem->gameCount = 135 * 60;//ゲームカウントをStage1ループから脱出させる

				//残存エネミーの破壊					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				//ステージ番号を変更
				gameSystem->stageNum = 2;
				gameSystem->cameraBossCount = 0;
				gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//道中BGMを止める
			}
		}
	}
}
void Boss1::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss1->RenderShadowMap(&md);

	//ボスのウェーブ攻撃描画
	for (int i = 0; i < boss1Wave.size(); i++)
	{
		gm->gm->DangerBlock->RenderShadowMap(&boss1Wave[i].md);
	}

	//ボスのウェーブ壊れエフェクト
	for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
		for (int h = 0; h < boss1WaveDeathEffect[i].angle.size(); h++) {
			MODEL_DATA md;
			Pos2 tmp = GetCirclePoint(boss1WaveDeathEffect[i].md.pos.X, boss1WaveDeathEffect[i].md.pos.Z, boss1WaveDeathEffect[i].r, boss1WaveDeathEffect[i].angle[h]);
			md.pos.X = tmp.p[0];
			md.pos.Z = tmp.p[1];
			md.pos.Y = boss1WaveDeathEffect[i].md.pos.Y - boss1WaveDeathEffect[i].count * 0.05f;
			md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			float size = (float)GetRand(100, 200) / 1000.0f;
			md.scale = Vector3(size, size, size);

			gm->gm->DangerBlock->RenderShadowMap(&md);
		}
	}
}
void Boss1::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	//危険な場所の描画
	MODEL_DATA _md;
	switch (animeNum) {
	case 1://FRONT_ATTACK
		_md.scale = Vector3(0.35, 0.35, 0.35);
		for (int x = 2; x <= 6; x++) {

			for (int y = 0; y <= 4; y++) {
				_md.pos.X = (x - 4) * 0.66;
				_md.pos.Z = (y - 4) * -0.66;

				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 4://TACKLE_ATTACK
		_md.scale = Vector3(0.35, 0.35, 0.35);
		for (int x = 2; x <= 6; x++) {
			for (int y = 0; y <= 8; y++) {
				_md.pos.X = (x - 4) * 0.66;
				_md.pos.Z = (y - 4) * -0.66;
				_md.pos.Y = 0.001;

				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 5://WAVE_ATTACK

		break;
	}

	//ボスの描画
	if (animeNum == 0 && deathCount >= 60) {
		gm->gm->Boss1->Animation(animeNum, 0);//死亡モーションのストップ
	}
	else {
		if(gameSystem->pause==false)
			gm->gm->Boss1->Animation(animeNum, 1);
		else
			gm->gm->Boss1->Animation(animeNum, 0);
	}

	gm->gm->Boss1->RenderShadowModel(&md);

	//ボスのウェーブ攻撃描画
	for (int i = 0; i < boss1Wave.size(); i++)
	{
		gm->gm->DangerBlock->RenderShadowModel(&boss1Wave[i].md);
	}
	//ボスのウェーブ壊れエフェクト
	for (int i = 0; i < boss1WaveDeathEffect.size(); i++) {
		for (int h = 0; h < boss1WaveDeathEffect[i].angle.size(); h++) {
			MODEL_DATA md;
			Pos2 tmp = GetCirclePoint(boss1WaveDeathEffect[i].md.pos.X, boss1WaveDeathEffect[i].md.pos.Z, boss1WaveDeathEffect[i].r, boss1WaveDeathEffect[i].angle[h]);
			md.pos.X = tmp.p[0];
			md.pos.Z = tmp.p[1];
			md.pos.Y = boss1WaveDeathEffect[i].md.pos.Y - boss1WaveDeathEffect[i].count * 0.05f;
			md.degree = Vector3(GetRand(0, 360), GetRand(0, 360), GetRand(0, 360));
			float size = (float)GetRand(100, 200) / 1000.0f;
			md.scale = Vector3(size, size, size);

			gm->gm->DangerBlock->RenderShadowModel(&md);
		}
	}
}



void Boss2::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 1, 4);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 5;
	HP_MAX = 3000;//初期HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//攻撃間隔
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss2::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//一定時間おきにプレイヤーの位置を見て行動を決める
						//一定時間おきにプレイヤーの位置を見て行動を決める
		if (animeNum == 1||animeNum==6) {
			attackCount++;
		}
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;

			//パターンなし、完全にランダムで
			int parsent = GetRand(1, 100);
			if (parsent < 40) {
				animeNum = 2;
			}
			else if (parsent < 70) {
				animeNum = 3;
			}
			else {
				animeNum = 4;
			}

		}

		switch (animeNum) {
		case 0://死亡
			break;
		case 1://待機
			break;
		case 2://ホーミング
			animeCount++;
			if (animeCount == 30) {
				for (int h = 0; h < 2; h++) {
					for (int i = 0; i < 7; i++) {
						float dist = 0.66;
						Pos2 tmpPos;

						if (i - 3 == 0) {
							continue;
						}

						tmpPos.p[0] = dist * (i - 3);


						tmpPos.p[1] = 4;

						float speed = 0.09;
						if (i < 4)
							enemySystem->AddEnemy(100, 0.005, tmpPos, Homing_Bezier, speed, 0.001, 0, i * 15+(h*7*15));
						else
							enemySystem->AddEnemy(100, 0.005, tmpPos, Homing_Bezier, speed, 0.001, 1, i * 15 + (h * 7 * 15));
					}
				}
			}
			for (int i = 0; i < 14; i++) {
				if (animeCount == 30 + i * 15) {
					gm->gs->Boss2Homing.Play(false);
				}		
			}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 3://レーザー縦
			animeCount++;

			if (animeCount % 15 == 0) {
				gm->gs->Boss2Laser.Play(false);
				int rand[4] = { 100,100,100,100 };
				for (int i = 0; i < 4; i++) {
					float dist = 0.66;
					Pos2 tmpPos;

					int rand2 = NULL;
					do {
						rand2 = (GetRand(0, 8) - 4);
					} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

					tmpPos.p[0] = dist * rand2;
					tmpPos.p[1] = 4;

					enemySystem->AddEnemy(100, 0.05, tmpPos, DownMove);

					rand[i] = rand2;
				}
			}

			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 4://レーザー横
			animeCount++;
			if (animeCount % 15 == 0) {
				gm->gs->Boss2Laser.Play(false);
				int rand[4] = { 100,100,100,100 };
				for (int i = 0; i < 3; i++) {
					float dist = 0.66;
					Pos2 tmpPos;

					int rand2 = NULL;
					do {
						rand2 = (GetRand(0, 8) - 4);
					} while (rand2 == rand[0] || rand2 == rand[1] || rand2 == rand[2]);

					tmpPos.p[0] = 4;
					tmpPos.p[1] = dist * rand2;

					enemySystem->AddEnemy(100, 0.04, tmpPos, LeftMove);
					tmpPos.p[0] = -4;
					enemySystem->AddEnemy(100, 0.04, tmpPos, RightMove);

					rand[i] = rand2;
				}
			}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 5://登場
			animeCount++;
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 6://ダメージを受けた時
			animeCount++;
			if (animeCount >= 30) {
				animeCount = 0;
				animeNum = 1;
			}
		}

		//プレイヤー攻撃エフェクトとの当たり判定
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;
					point.p[1] = md.pos.Y-1;
					point.p[2] = md.pos.Z-1;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);

						//くらいモーション
						if (animeNum == 1) {

							animeNum = 6;
						}
						break;

					}
				}
			}
		}
		//無敵時間
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//死亡処理
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//死亡処理（アニメーションストップ）
		deathCount++;
		if (deathCount > 70) {
			deathCount = 70;
		}
	}

	//ステージ番号操作
	gameSystem->stageNum = 3;
	//BOSS1の出現時のカメラの操作
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 5;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//道中BGMを止める
	}
	else {

		//出現時アニメーション
		if (gameSystem->cameraBossCount == 20) {
			animeNum = 1;
		}
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}
	//ボス死亡時に次の面へ移動する
	if (animeNum == 0) {
		//消えていく
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//完全に消えた時ボスの処理を停止する
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss2Exist = false;//完全なボスの停止
				enemySystem->gameCount = 295 * 60;//ゲームカウントをStage1ループから脱出させる

				//残存エネミーの破壊					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				//ステージ番号を変更
				gameSystem->stageNum = 4;
				gameSystem->cameraBossCount = 0;
				gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//道中BGMを止める
			}
		}
	}
}
void Boss2::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss2->RenderShadowMap(&md);
}
void Boss2::Draw() {
	GameSystem* gameSystem = GameSystem::GetInstance();
	GameManager* gm = GameManager::GetInstance();
	if (animeNum == 0 && deathCount >= 69) {
		gm->gm->Boss2->Animation(animeNum, 0);//死亡モーションのストップ
	}
	else {
		if(gameSystem->pause==false)
			gm->gm->Boss2->Animation(animeNum, 1);
		else
			gm->gm->Boss2->Animation(animeNum, 0);
	}
	gm->gm->Boss2->RenderShadowModel(&md);
}

void Boss3::Reset() {
	float dist = 250.0f;
	md.pos = Vector3(0, 0, 5);
	md.scale = Vector3(1.0, 1.0, 1.0) / dist;
	md.degree = Vector3(0, 180, 0);
	md.multiplyColor.w = 1;

	animeNum = 6;
	HP_MAX = 4000;//初期HP
	HP = HP_MAX;

	attackCount = 0;
	attackCountMax = 180;//攻撃間隔
	animeCount = 0;

	death = false;
	deathCount = 0;

	damageCount = 0;
}
void Boss3::Updata() {
	GameManager* gm = GameManager::GetInstance();
	Player* player = Player::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	EnemySystem* enemySystem = EnemySystem::GetInstance();
	BossSystem* bossSystem = BossSystem::GetInstance();

	if (animeNum != 0) {
		//一定時間おきにプレイヤーの位置を見て行動を決める
		

		if (animeNum == 1||animeNum==7) {
			attackCount++;
		}
		//攻撃にはいる
		if (attackCount >= attackCountMax) {
			animeCount = 0;
			attackCount = 0;

			//右側３マスいないだと右手振り下ろし
			if (player->x > 5) {
				animeNum = 3;
			}
			//左側３マスいないだと左手振り下ろし
			else if (player->x < 3) {
				animeNum = 2;
			}
			//中心３マスだとレーザー攻撃
			else if (3 <= player->x && player->x <= 5) {
				animeNum = 4;
			}
			//ランダムでサンダー
			if (GetRand(1, 100) <= 15) {
				animeNum = 5;
			}
		}

		switch (animeNum) {
		case 0://死亡
			break;
		case 1://待機
			break;
		case 2://左側攻撃
			//プレイヤーへのダメージ処理
			if (animeCount == 45) {
				gm->gs->Boss3Attack.Play(false);
				if (player->x <= 3 && player->damageCount == 0) {
					player->damage = true;
				}
			}


			animeCount++;
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 3://右側攻撃

			//プレイヤーへのダメージ処理
			if (animeCount == 45) {
				gm->gs->Boss3Attack.Play(false);
				if (player->x >= 5 && player->damageCount == 0) {
					player->damage = true;
				}
			}
			animeCount++;

			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 4://レーザー
			animeCount++;
			if (animeCount == 60) {
				gm->gs->Boss3Laser.Play(false);
				for (int i = 0; i < 10; i++) {
					for (int h = 0; h < 8; h++) {
						Pos2 tmp = GetCirclePoint(0, 1, 1, h * 45);

						Enemy ene(100, 0.4, Vector3(tmp.p[0],tmp.p[1],3), DownMove);
						ene.startFrame = i * 8;
						enemySystem->AddEnemy(ene);
					}
				}
			}
			if (animeCount >= 60&& animeCount <= 80) {
				if (2 <= player->x && player->x <= 6&&player->damageCount==0) {
					player->damage = true;
				}		
			}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 5://サンダー
			animeCount++;
			if (animeCount == 30) {
				gm->gs->Boss3Thunder.Play(false);
				for (int i = 0; i < 10; i++) {
					for (int _x = -2; _x <= 2; _x++) {
						float rand = (GetRand(0, 10)-5)*0.1f;
						for (int _y = -2; _y <= 2; _y++) {
							
							float dist = 0.66;
							
							Enemy ene(100, 0.2, Vector3(_x * dist, -1+rand, _y * dist), AntiGravity);
							ene.startFrame = i * 10;
							ene.effectDataSize = 2;
							enemySystem->AddEnemy(ene);
						}
					}
				}
			}

			//if (animeCount == 60) {
			//	if (2 <= player->x && player->x <= 6 && 2 <= player->y && player->y <= 6) {
			//		player->damage = true;
			//	}
			//}
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 6://登場
			animeCount++;
			//アニメーションを戻す
			if (animeCount >= animeConstFrame[animeNum]) {
				animeCount = 0;
				animeNum = 1;
			}
			break;
		case 7://ダメージを受けた時
			animeCount++;
			if (animeCount >= 30) {
				animeCount = 0;
				animeNum = 1;
			}
		}

		//プレイヤー攻撃エフェクトとの当たり判定
		if (damageCount == 0) {

			for (int i = 0; i < player->playerAttackEffect.size(); i++) {
				for (int h = 0; h < 3; h++) {
					Pos3 center;
					center.p[0] = player->playerAttackEffect[i].md[h].pos.X;
					center.p[1] = player->playerAttackEffect[i].md[h].pos.Y;
					center.p[2] = player->playerAttackEffect[i].md[h].pos.Z;
					Pos3 point;
					point.p[0] = md.pos.X;//判定を調整できる
					point.p[1] = md.pos.Y;
					point.p[2] = md.pos.Z-2;
					Pos3 size;
					size.p[0] = 3;
					size.p[1] = 3;
					size.p[2] = 1.3;

					if (HitRect3D(point, size, center)) {
						damageCount = 1;
						HP -= player->power;
						gm->gs->EnemyDead.Play(false);

						//くらいモーション
						if (animeNum == 1) {

							animeNum = 7;
						}
						break;

					}
				}
			}
		}
		//無敵時間
		if (damageCount > 0) {
			damageCount++;
			if (damageCount > 30) {
				damageCount = 0;
			}
		}

		//死亡処理
		if (HP <= 0) {
			animeNum = 0;
			gm->gs->Dead.Play(false);
		}
	}
	else {
		//死亡処理（アニメーションストップ）
		deathCount++;
		if (deathCount > 90) {
			deathCount = 90;
		}
	}

	//ステージ番号操作
	gameSystem->stageNum = 5;
	//BOSS1の出現時のカメラの操作
	float speed = 0.1f;
	if (gameSystem->cameraDistY > -4 && gameSystem->cameraBossCount == 0) {
		animeNum = 6;
		gameSystem->cameraDistY -= speed;

		gm->gs->BGM[5 + gameSystem->stageNum - 1].Stop();//道中BGMを止める
	}
	else {

		//出現時アニメーション
		if (gameSystem->cameraBossCount > 120) {
			if (gameSystem->cameraDistY < 0) {
				gameSystem->cameraDistY += speed;
			}
		}
		else {
			gameSystem->cameraBossCount++;
		}
	}

	//ボス死亡時に次の面へ移動する
	if (animeNum == 0) {
		//消えていく
		if (md.multiplyColor.w > 0) {
			md.multiplyColor.w -= 0.01;
			//完全に消えた時ボスの処理を停止する
			if (md.multiplyColor.w <= 0) {
				bossSystem->boss3Exist = false;//完全なボスの停止
				enemySystem->gameCount = 1500 * 60;//ゲームカウントをStage1ループから脱出させる

				//残存エネミーの破壊					
				for (int i = 0; i < enemySystem->enemy.size(); i++) {
					enemySystem->enemy[i].death = true;
				}
				gameSystem->cameraBossCount = 0;


			}
		}
	}
}
void Boss3::DrawShadow() {
	GameManager* gm = GameManager::GetInstance();
	gm->gm->Boss3->RenderShadowMap(&md);
}
void Boss3::Draw() {
	GameSystem* gameSystem = GameSystem::GetInstance();
	GameManager* gm = GameManager::GetInstance();

	MODEL_DATA _md;
	_md.scale = Vector3(0.35, 0.35, 0.35);
	switch (animeNum) {
	case 2://左手攻撃
		for (int _x = -4; _x <= -1; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 3://右手攻撃
		for (int _x = 1; _x <= 4; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 4://レーザー攻撃
		for (int _x = -2; _x <= 2; _x++) {
			for (int _y = -4; _y <= 4; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	case 5://サンダー
		for (int _x = -2; _x <= 2; _x++) {
			for (int _y = -2; _y <= 2; _y++) {
				_md.pos.X = _x * 0.66;
				_md.pos.Z = _y * 0.66;
				_md.pos.Y = 0.001;
				gm->gm->DangerBlock->RenderShadowModel(&_md);
			}
		}
		break;
	}
	

	
	if (animeNum == 0 && deathCount >= 89) {
		gm->gm->Boss3->Animation(animeNum, 0);//死亡モーションのストップ
	}
	else {
		if (gameSystem->pause == false)
			gm->gm->Boss3->Animation(animeNum, 1);
		else
			gm->gm->Boss3->Animation(animeNum, 0);
	}
	gm->gm->Boss3->RenderShadowModel(&md);
}
void BossSystem::Reset() {
	boss1.Reset();
	boss2.Reset();
	boss3.Reset();
	boss1Exist = false;
	boss2Exist = false;
	boss3Exist = false;
}
void BossSystem::Updata() {
	if (boss1Exist) {
		boss1.Updata();
	}
	if (boss2Exist) {
		boss2.Updata();
	}
	if (boss3Exist) {
		boss3.Updata();
	}

}
void BossSystem::DrawShadow() {
	if (boss1Exist) {
		boss1.DrawShadow();
	}
	if (boss2Exist) {
		boss2.DrawShadow();
	}
	if (boss3Exist) {
		boss3.DrawShadow();
	}
	}
	
	
	
void BossSystem::Draw() {
	GameManager* gm = GameManager::GetInstance();
	GameSystem* gameSystem = GameSystem::GetInstance();
	DirectGraphics* dg=DirectGraphics::GetInstance();
	//ボスのHPのUI（HPバーの描画） 処理が若干特殊なので２D描画だがここに書き込むことに
	if (boss1Exist || boss2Exist || boss3Exist) {
		float bossHP = NULL;
		float bossHP_MAX = NULL;
		float bossDamageBlur = 0.0f;
		if (boss1Exist) {
			bossHP = boss1.HP;
			bossHP_MAX = boss1.HP_MAX;
			if (boss1.damageCount != 0 && boss1.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}


		}
		if (boss2Exist) {
			bossHP = boss2.HP;
			bossHP_MAX = boss2.HP_MAX;
			if (boss2.damageCount != 0 && boss2.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}
		}
		if (boss3Exist) {
			bossHP = boss3.HP;
			bossHP_MAX = boss3.HP_MAX;
			if (boss3.damageCount != 0 && boss3.damageCount < 10) {
				bossDamageBlur = (float)GetRand(1, 100);
			}
		}
		//ボスの被弾時に揺らす処理
			//定数バッファをシェーダーに設定
		dg->GetConstantBufferData()->multiplyColor.z = 0;
		dg->GetConstantBufferData()->multiplyColor.w = 0;
		dg->GetConstantBufferData()->multiplyColor.y = 0;
		dg->GetConstantBufferData()->multiplyColor.x = ((bossHP_MAX / 2) / bossHP);
		if ((bossHP_MAX / 2) > bossHP) {
			dg->GetConstantBufferData()->multiplyColor.y = -((bossHP_MAX / 2) - bossHP)/ (bossHP_MAX / 2);
		}
		
		

		dg->GetContext()->UpdateSubresource(dg->GetConstantBuffer(), 0, NULL, dg->GetConstantBufferData(), 0, 0);
		ID3D11Buffer* constant_buffer = dg->GetConstantBuffer();
		//dg->GetContext()->VSSetConstantBuffers(0, 1, &constant_buffer);
		//dg->GetContext()->GSSetConstantBuffers(0, 1, &constant_buffer);
		dg->GetContext()->PSSetConstantBuffers(0, 1, &constant_buffer);

		gm->gi->hpBarBase.Draw(960 + bossDamageBlur, 150 + bossDamageBlur, 0, 1, 1 * ((float)gameSystem->cameraBossCount / 120.0f * 2));
		int tmp = 1920 - (1.0f - bossHP / bossHP_MAX) * 1920.0f;
		gm->gi->hpBar.DrawEX(960 + bossDamageBlur, 150 + bossDamageBlur, 0, 1, 1, 0.7 * ((float)gameSystem->cameraBossCount / 120.0f * 2), 1, false, false, true, RECT{ 0,0,tmp,225 });
	}

	//各ボスの３D描画
	if (boss1Exist) {
		boss1.Draw();
	}
	if (boss2Exist) {
		boss2.Draw();
	}
	if (boss3Exist) {
		boss3.Draw();
	}
}
