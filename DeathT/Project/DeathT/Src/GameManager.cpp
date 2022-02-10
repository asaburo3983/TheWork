#include "GameManager.h"

void GameManager::AllLoad() {

	gm = GameModel::GetInstance();
	gi = GameImage::GetInstance();
	gs = GameSound::GetInstance();

	KEY = KeyInput::GetInstance();
	MOUSE = MouseInput::GetInstance();

	char shaderV[100] = "Data/Shader/VertexShader.cso";
	char shaderV_A[100] = "Data/Shader/VertexShader_Anime.cso";
	char shaderV_I[100] = "Data/Shader/VertexShader_Instance.cso";

	char shaderG[100] = "Data/Shader/GeometryShader.cso";
	char shaderP[100] = "Data/Shader/PixelShader.cso";
	//char shaderP[100] = "../x64/Debug/PixelShader.cso";

	char shaderP_NS[100] = "Data/Shader/PixelShader_NoShadow.cso";

	gm->Player->Load("Data/Model/Player.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//プレイヤー　赤
	gm->Player2->Load("Data/Model/Player.fbx", "Data/Model/Tex/Player2/", shaderV, shaderG, shaderP);//プレイヤー　青

	gm->Block->Load("Data/Model/Block.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//タワーで使うブロック
	gm->Stage->Load("Data/Model/Stage.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//タワーで使うステージ

	gm->DangerBlock->Load("Data/Model/DangerBlock.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//ボスの攻撃予測	
	

	gm->Enemy[0]->Load("Data/Model/enemy0.fbx", "Data/Model/Tex/", shaderV_I, shaderG, shaderP);//ザコ敵　ステージで変える
	gm->Enemy[1]->Load("Data/Model/enemy1.fbx", "Data/Model/Tex/", shaderV_I, shaderG, shaderP);
	gm->Enemy[2]->Load("Data/Model/enemy2.fbx", "Data/Model/Tex/", shaderV_I, shaderG, shaderP);

	gm->Boss1->Load("Data/Model/Boss1.fbx", "Data/Model/Tex/", shaderV_A, shaderG, shaderP);//ボス
	gm->Boss2->Load("Data/Model/Boss2.fbx", "Data/Model/Tex/", shaderV_A, shaderG, shaderP);//ボス
	gm->Boss3->Load("Data/Model/Boss3.fbx", "Data/Model/Tex/", shaderV_A, shaderG, shaderP);//ボス


	gm->Item[0]->Load("Data/Model/LifeItem.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//ボス
	gm->Item[1]->Load("Data/Model/BomItem.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);//ボス

	gm->playerAttackEffect_Red->Load("Data/3DEffect/redEffect.fbx", "Data/3DEffect/Tex/", shaderV, shaderG, shaderP);
	gm->playerAttackEffect_Blue->Load("Data/3DEffect/blueEffect.fbx", "Data/3DEffect/Tex/", shaderV, shaderG, shaderP);
	gm->playerAttackEffect_RedRed->Load("Data/3DEffect/redEffect.fbx", "Data/Model/Tex/", shaderV, shaderG, shaderP);


	//画像/////////////////////////////////////////////////

	gi->numbarFont[0].Load("Data/Image/NumbarFont/0.png");
	gi->numbarFont[1].Load("Data/Image/NumbarFont/1.png");
	gi->numbarFont[2].Load("Data/Image/NumbarFont/2.png");
	gi->numbarFont[3].Load("Data/Image/NumbarFont/3.png");
	gi->numbarFont[4].Load("Data/Image/NumbarFont/4.png");
	gi->numbarFont[5].Load("Data/Image/NumbarFont/5.png");
	gi->numbarFont[6].Load("Data/Image/NumbarFont/6.png");
	gi->numbarFont[7].Load("Data/Image/NumbarFont/7.png");
	gi->numbarFont[8].Load("Data/Image/NumbarFont/8.png");
	gi->numbarFont[9].Load("Data/Image/NumbarFont/9.png");

	gi->title.Load("Data/Image/title.png");
	gi->feedBlack.Load("Data/Image/feedBlack.png");
	gi->feedWhite.Load("Data/Image/feedWhite.png");
	gi->startStr.Load("Data/Image/startStr.png");
	gi->optionStr.Load("Data/Image/optionStr.png");
	gi->endStr.Load("Data/Image/endStr.png");

	gi->gameBack.Load("Data/Image/gameBack.png");
	gi->bar.Load("Data/Image/bar.png");
	gi->barIcon.Load("Data/Image/barIcon.png");
	gi->barIcon2.Load("Data/Image/barIcon2.png");

	gi->score.Load("Data/Image/Score.png");
	gi->clearBack.Load("Data/Image/clearBack.png");

	gi->bom.Load("Data/Image/Bom.png");

	gi->option.Load("Data/Image/option.png");
	gi->optionUI.Load("Data/Image/optionUI.png");;
	gi->optionCursor.Load("Data/Image/optionCursor.png");;
	gi->optionBack.Load("Data/Image/optionBack.png");

	gi->select.Load("Data/Image/select.png");
	gi->selectBattle.Load("Data/Image/selectBattle.png");
	gi->selectTower.Load("Data/Image/selectTower.png");
	gi->selectTitle.Load("Data/Image/selectTitle.png");
	gi->selectOption.Load("Data/Image/selectOption.png");

	gi->battle[0].Load("Data/Image/Battle.png");
	gi->battle[1].Load("Data/Image/Battle2.png");
	gi->battle[2].Load("Data/Image/Battle3.png");

	gi->hpBar.LoadShader("Data/Shader/PixelShader_2D.cso");
	gi->hpBar.Load("Data/Image/HPBar.png");

	gi->hpBarBase.Load("Data/Image/HPBarBase.png");

	gi->stage1Clear.Load("Data/Image/stage1Clear.png");

	gi->pause.Load("Data/Image/Pause.png");
	gi->pause_Back.Load("Data/Image/Pause_Back.png");
	gi->pause_Select.Load("Data/Image/Pause_Select.png");

	gi->pause_SelectConfirm.Load("Data/Image/Pause_SelectConfirm.png");
	gi->pause_SelectYes.Load("Data/Image/Pause_SelectYes.png");
	gi->pause_SelectNo.Load("Data/Image/Pause_SelectNo.png");

	gi->cleditBack.Load("Data/Image/Credit/CreditBack.png");
	gi->cleditPhoto[0].Load("Data/Image/Credit/0.png");
	gi->cleditPhoto[1].Load("Data/Image/Credit/1.png");
	gi->cleditPhoto[2].Load("Data/Image/Credit/2.png");
	gi->cleditPhoto[3].Load("Data/Image/Credit/3.png");
	gi->cleditPhoto[4].Load("Data/Image/Credit/4.png");
	gi->cleditPhoto[5].Load("Data/Image/Credit/5.png");
	gi->cleditPhoto[6].Load("Data/Image/Credit/6.png");


	////////////////////////////////////////////////////////////

	//BGM////////////////////////////////////////////////
	gs->BGM[0].Load("Data/Sound/BGM/Title.mp3");
	gs->BGM[1].Load("Data/Sound/BGM/Select.mp3");
	gs->BGM[2].Load("Data/Sound/BGM/Option.mp3");
	gs->BGM[3].Load("Data/Sound/BGM/Tower.mp3");
	gs->BGM[4].Load("Data/Sound/BGM/Tower_Clear.mp3");

	gs->BGM[5].Load("Data/Sound/BGM/Battle1.mp3");
	gs->BGM[6].Load("Data/Sound/BGM/Battle1_Boss.mp3");
	gs->BGM[7].Load("Data/Sound/BGM/Battle2.mp3");
	gs->BGM[8].Load("Data/Sound/BGM/Battle2_Boss.mp3");
	gs->BGM[9].Load("Data/Sound/BGM/Battle3.mp3");
	gs->BGM[10].Load("Data/Sound/BGM/Battle3_Boss.mp3");
	gs->BGM[11].Load("Data/Sound/BGM/Credit.mp3");

	/////////////////////////////////////////////////////

	//SE////////////////////////////////////////////////
	gs->MovePlayer.Load("Data/Sound/SE/MoveSE.mp3");
	gs->SetBlock.Load("Data/Sound/SE/SetSE.mp3");
	gs->Enter.Load("Data/Sound/SE/EnterSE.mp3");
	gs->Dead.Load("Data/Sound/SE/DeadSE.mp3");
	gs->MoveCursor.Load("Data/Sound/SE/MoveCursor.mp3");
	
	gs->Attack.Load("Data/Sound/SE/AttackSE.mp3");

	gs->Boss1FrontAttack.Load("Data/Sound/SE/Boss1Front.mp3");
	gs->Boss1Tackle.Load("Data/Sound/SE/Boss1Tackle.mp3");
	gs->Boss1Avalanche.Load("Data/Sound/SE/Boss1Avalanche.mp3");

	gs->Boss2Laser.Load("Data/Sound/SE/Boss2Laser.mp3");
	gs->Boss2Homing.Load("Data/Sound/SE/Boss2Homing.mp3");
	
	gs->Boss3Attack.Load("Data/Sound/SE/Boss3Attack.mp3");
	gs->Boss3Laser.Load("Data/Sound/SE/Boss3Laser.mp3");
	gs->Boss3Thunder.Load("Data/Sound/SE/Boss3Thunder.mp3");

	gs->EnemyDead.Load("Data/Sound/SE/EnemyDead.mp3");
	gs->PlayerDamage.Load("Data/Sound/SE/PlayerDamage.mp3");
	gs->PlayerDead.Load("Data/Sound/SE/PlayerDead.mp3");

	gs->ItemGet.Load("Data/Sound/SE/ItemGet.mp3");
	gs->Bom.Load("Data/Sound/SE/Bom.mp3");
	///////////////////////////////////////////////
}