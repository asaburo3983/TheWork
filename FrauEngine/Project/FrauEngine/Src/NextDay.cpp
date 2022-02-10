#include "NextDay.h"
#include "SoundManager.h"

void NextDay::Initialize() {

	auto rc = Resource::GetInstance();

	base.SetResource(rc->Image("MoneyBase.png"));
	base.SetAll(Vector2(960, 540), Vector2(2, 5));
	
	morningBase.SetResource(rc->Image("MoneyBase.png"));
	morningBase.SetAll(Vector2(960, 540), Vector2(2, 8));

	Vector2 dist(160, 100);
	Vector2 buttonScale(0.7, 1);

	yes.SetResource(rc->Image("MoneyBase.png"));
	yes.SetAll(Vector2(960-dist.X, 540+dist.Y), buttonScale);

	no.SetResource(rc->Image("MoneyBase.png"));
	no.SetAll(Vector2(960 + dist.X, 540 + dist.Y), buttonScale);

	for (int i = 0; i < 3; i++) {
		seed[0][i].SetResource(rc->Image("Tanepopo_Seed.png"));
		seed[1][i].SetResource(rc->Image("Masomaso_Seed.png"));
		seed[2][i].SetResource(rc->Image("Ranpopo_Seed.png"));
		seed[3][i].SetResource(rc->Image("Barabara_Seed.png"));
		seed[4][i].SetResource(rc->Image("SuggerPan_Seed.png"));
		seed[5][i].SetResource(rc->Image("Frau_Seed.png"));
	}
}
void NextDay::Update() {
	auto mouse = MouseInput::GetInstance();
	auto eventManager = EventManager::GetInstance();
	auto planterSystem = PlanterSystem::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto player = Player::GetInstance();

	//�m�F�p�̏���
	if (enable&& nightAlpha <= 0.1) {
		if (fade < 1) {
			fade += fadeSpeed;
		}
		//�{�^���Ŋm�F����������
		if (mouse->left == 1) {
			if (yes.Hit(mouse->x, mouse->y)) {
				night = true;
				SetEnable(false);
				sound->GetSE(SoundList_SE::NEXTDAY)->Play();
			}
			if (no.Hit(mouse->x, mouse->y)) {
				SetEnable(false);
				processEnable = false;
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
		}
		//�E�N���b�N�ł��L�����Z�����\
		if (mouse->right == 1) {
			SetEnable(false);
			processEnable = false;
			sound->GetSE(SoundList_SE::ENTER)->Play();
		}
	}
	else {
		if (fade > 0) {
			fade -= fadeSpeed;
		}
	}
	//���̓��ɍs���t�F�[�h		�����t�F�[�h
	if (night) {
		if (nightAlpha < nightAlphaMax) {
			nightAlpha += fadeSpeed;
		}
		else {
			night = false;
			morning = true;
			
			eventManager->AddDays();

			//���z�̑����������s��
			//�A�C�e���̏�����s��
			//��z������̐������s��
			planterSystem->SeedGrow();

			//�f�[�^���Z�[�u����
			Save::GetInsatnce()->SaveData();

			//�v���C���[�̈ʒu��ݒ肷��
			//�v���C���[�̈ړ��𐧌�����
			//player->SetPos(Vector3(1.0, -3.9, 0));
			player->SetPos(Vector3(4.5, -3.9, 5.2));
			player->SetAngle(Vector3(0, 180, 0));
			
		}
	}
	else {
		if (nightAlpha > 0) {
			nightAlpha -= fadeSpeed;
		}
	}
	//���̓��̏��߂̏���
	if (morning) {
		if (mouse->left == 1 && nightAlpha <= 0.1) {
			morning = false;
			eventManager->EndDays();
		
			//�v�����^�[�̒��g���k�ɂ���
			planterSystem->Reset();
			processEnable = false;//���ׂĂ̏������I��

			player->IsMove(true);

		}
		if (morningAlpha < 1) {
			morningAlpha += fadeSpeed;
		}
	}
	else {
		if (morningAlpha > 0) {
			morningAlpha -= fadeSpeed;
		}
	}

	base.SetAlpha(fade);
	yes.SetAlpha(fade);
	no.SetAlpha(fade);

}

void NextDay::Draw() {
	DrawCheck();
	DrawMorning();
}
void NextDay::DrawCheck() {
	if (fade <= 0) {
		return;
	}

	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	base.Draw();
	yes.Draw();
	no.Draw();
	Vector2 manualStrPos = base.GetPosLerp(Vector2(0.4, 0.2));
	akazukin->DrawString(str, manualStrPos, 2, Color(63, 41, 20, fade), true);

	akazukin->DrawString("�͂�", Vector2(yes.GetPos().X-15, yes.GetPos().Y-10), 2.5, Color(63, 41, 20, fade), true);
	akazukin->DrawString("������", Vector2(no.GetPos().X - 15, no.GetPos().Y-10), 2.5, Color(63, 41, 20, fade), true);
	
}
void NextDay::DrawMorning() {
	if (morningAlpha <= 0) {
		return;
	}
	auto item = Item::GetInstance();
	auto planterSystem = PlanterSystem::GetInstance();
	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	Color strColor = Color(63, 41, 20, morningAlpha);
	//�x�[�X�̕`��i�C���[�W
	morningBase.SetAlpha(morningAlpha);
	morningBase.Draw();

	//�{���̐���(������
	Vector2 manualStrPos = morningBase.GetPosLerp(Vector2(0.5, 0.2));
	akazukin->DrawString("�{���̐��ʁI", manualStrPos, 3, Color(63, 41, 20, morningAlpha), true);
	//��{����
	Vector2 standardMoneyStrPos = morningBase.GetPosLerp(Vector2(0.4, 0.25));
	meirio->DrawString("��{����:100���E",standardMoneyStrPos, 1.7, strColor,true);
	//
	Vector2 matiTitlePos = morningBase.GetPosLerp(Vector2(0.4, 0.6));
	meirio->DrawString("�Ԃ���Ƃꂽ��", matiTitlePos, 1.7, strColor, true);
	//�琬�ہi������
	for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
		int setItem = planterSystem->GetSetItem(i);
		if (setItem == -1) {
			continue;
		}
		bool seedGrow = planterSystem->GetGrouSeedEnable(i);
		string matingSeedStr = "";
		if (planterSystem->GetMatingSeed(i) != -1) {
			matingSeedStr = item->GetItemName(planterSystem->GetMatingSeed(i));
		}


		Vector2 seedNamePos = morningBase.GetPosLerp(Vector2(0.1,  0.4+(i*0.05)));
		Vector2 seedGrowPos = morningBase.GetPosLerp(Vector2(0.4,  0.4+(i*0.05)));
		Vector2 seedMoneyPos = morningBase.GetPosLerp(Vector2(0.65, 0.4+(i*0.05)));

		float strScale = 1.5f;
		//������///////////////////////
		//�琬������
		string setSeedStr = item->GetItemName(setItem);
		meirio->DrawString(setSeedStr, seedNamePos, strScale, strColor);
		//����or���s
		string growStr = "�琬���s";
		if (seedGrow) {
			growStr = "�琬�����I";
		}
		meirio->DrawString(growStr,seedGrowPos, strScale, strColor);
		//�킲�Ƃ̎���
		char moneyStr[50] = "0���E";
		if (seedGrow) {
			sprintf_s(moneyStr,"%d���E", item->GetSell(setItem));
		}
		meirio->DrawString(moneyStr, seedMoneyPos, strScale, strColor);
		/////////////////////////////////
		
		//��z�ŃQ�b�g������
		//������///////////////
		Vector2 matiSeedPos = morningBase.GetPosLerp(Vector2(0.25 + i * 0.25, 0.80f));

		for (int h = 0; h < 6; h++) {
			seed[h][i].SetAll(matiSeedPos, Vector2(0.5, 0.5), 0, morningAlpha);
		}
		int getSeed = planterSystem->GetMatingSeed(i);
		if (getSeed == 1) {
			seed[0][i].Draw();
		}
		else if (getSeed != -1) {
			seed[getSeed - 6][i].Draw();
		}

		Vector2 matiSeedPosStr = morningBase.GetPosLerp(Vector2(0.25 + i * 0.25, 0.7));
		meirio->DrawString(matingSeedStr, matiSeedPosStr, strScale, strColor, true);
		///////////////////////
	}
	
	
	

}