#include "PlanterSystem.h"
#include "SoundManager.h"
void PlanterSystem::Reset() {
	for (int i = 0; i < 3; i++) {
		planterSetItem[i] = -1;
		bug[i] = 0;//���ۂ̈琬���s��		
		bugFix[i] = 0;//�琬���s���h�~�i�ʁj
		bugSurface[i] = 0;//�琬���s���i�`��p�j
		growSeed[i] = false;
		matingSeed[i] = -1;
	}
	bugFixSum = 0;//�琬���s���h�~�i���v�j


}
void PlanterSystem::SetEnable(bool _enable) {
	auto mouse = MouseInput::GetInstance();
	auto player = Player::GetInstance();
	auto sound = SoundManager::GetInstance();
	if (enable == false) {
		
		selectItemNum = -1;

	}
	enable = _enable;
	player->IsMove(!enable);
	sound->GetSE(SoundList_SE::ENTER)->Play();
}
void PlanterSystem::Initialize() {
	auto rc = Resource::GetInstance();
	
	itemBase.SetResource(rc->Image("ItemBase.png"));
	itemBase.SetAll(Vector2(1575, 620));
	for (int i = 0; i < 7; i++) {
		itemBaseLine[i].SetResource(rc->Image("ItemBaseLine.png"));
		int y = 340 + itemBaseLine[i].GetHeight() * i;
		itemBaseLine[i].SetAll(Vector2(1575, y));
	}

	for (int i = 0; i < 3; i++) {
		manualBase[i].SetResource(rc->Image("ManualBase.png"));
	}
	manualBase[0].SetAll(Vector2(665, 300), Vector2(0.7, 0.7));
	manualBase[1].SetAll(Vector2(350, 740), Vector2(0.7, 0.7));
	manualBase[2].SetAll(Vector2(980, 740), Vector2(0.7, 0.7));

	selectFrame.SetResource(rc->Image("SelectFrame.png"));
	rightArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetResource(rc->Image("RightArrow.png"));
	leftArrow.SetTurn(true, false);
	rightArrow.SetAll(Vector2(1800, 960), Vector2(0.5, 0.5), 0, 0);
	leftArrow.SetAll(Vector2(1350, 960), Vector2(0.5, 0.5), 0, 0);
	for (int i = 0; i < 3; i++) {
		items[i][0].SetResource(rc->Image("Maso_Seed.png"));
		items[i][1].SetResource(rc->Image("Tanepopo_Seed.png"));
		items[i][2].SetResource(rc->Image("Beranda_Seed.png"));
		items[i][3].SetResource(rc->Image("Panda_Seed.png"));
		items[i][4].SetResource(rc->Image("Rose_Seed.png"));
		items[i][5].SetResource(rc->Image("Sugger_Seed.png"));
		items[i][6].SetResource(rc->Image("Cosmo_Seed.png"));
		items[i][7].SetResource(rc->Image("Masomaso_Seed.png"));
		items[i][8].SetResource(rc->Image("Ranpopo_Seed.png"));
		items[i][9].SetResource(rc->Image("Barabara_Seed.png"));
		items[i][10].SetResource(rc->Image("SuggerPan_Seed.png"));
		items[i][11].SetResource(rc->Image("Frau_Seed.png"));
	}
}
void PlanterSystem::Update() {
	auto item = Item::GetInstance();
	auto mouse = MouseInput::GetInstance();
	auto player = Player::GetInstance();

	//�L��������
	if (enable) {
		if (alpha < 1.0f) {
			alpha += alphaSpeed;
		}

		if (mouse->left == 1) {
			//�y�[�W���菈��
			if (rightArrow.Hit(mouse->x, mouse->y)) {
				page++;
				if (page > 2) {
					page = 1;
				}
			}
			else if (leftArrow.Hit(mouse->x, mouse->y)) {
				page--;
				if (page < 1) {
					page = 2;
				}
			}
			//�A�C�e���I��
			for (int i = 0; i < 7; i++) {
				if (itemBaseLine[i].Hit(mouse->x, mouse->y)) {
					selectItemNum = i;
					break;
				}
			}
			if (selectItemNum + (page - 1) * 7 > 11) {
				selectItemNum = 4;
			}
			//�v�����^�[�ɃA�C�e�����Z�b�g

			for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
				int num = selectItemNum + (page - 1) * 7;
				//����ς݂ł����
				if (manualBase[i].Hit(mouse->x, mouse->y)) {

					//�Z�b�g����Ă�������炵�Ă�1�ɂȂ�Ȃ���Γ�����
					int sameItem = 0;
					for (int h = 0; h < 3; h++) {
						if (i == h) {
							continue;
						}
						if (planterSetItem[h] == num) {
							sameItem++;
						}
					}
					//�Z�b�g����Ă��铯���A�C�e��������ꍇ�A���������Z�b�g���������Ȃ�Ȃ��悤�ɂ���
					if (item->GetItemNum(num)-sameItem < 1) {
						continue;
					}
					

					planterSetItem[i] = num;

					bug[i] = item->GetBug(num);
					bugSurface[i] = bug[i];
					bugFix[i] = item->GetNotBug(num);
					bugFixSum = bugFix[0] + bugFix[1] + bugFix[2];
					//�A�C�e���u�o�O�t�B�b�N�X�v������ꍇ���̕t���₷�����T��������
					if (item->GetItemNum(13) >= 1) {
						bugFixSum += item->GetItemNum(13) * 5;
					}
				}
			}
			
		}
		if (mouse->right == 1 && player->GetModel()->GetAnimeNum() == 4) {
			SetEnable(false);
		}
		//�`��p�̈琬���s���̌v�Z
		for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
			int anser = bug[i] - bugFixSum;
			if (bugSurface[i] > anser && bugSurface[i] > 0) {
				bugSurface[i]--;
			}
			else if (bugSurface[i] < anser) {
				bugSurface[i]++;
			}				
		}
	}
	else {
		if (alpha > 0.0f) {
			alpha -= alphaSpeed;
		}
	}
	//���ݒ�
	itemBase.SetAlpha(alpha);
	for (int i = 0; i < 7; i++) {
		itemBaseLine[i].SetAlpha(alpha);
	}
	for (int i = 0; i < 3; i++) {
		manualBase[i].SetAlpha(alpha);
	}
	selectFrame.SetAll(Vector2(1575, 340 + selectItemNum * itemBaseLine[0].GetHeight()), Vector2(1, 1), 0, alpha);
	rightArrow.SetAlpha(alpha);
	leftArrow.SetAlpha(alpha);
}
void PlanterSystem::Draw() {
	Item* item = Item::GetInstance();
	auto akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);
	auto meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);

	if (alpha > 0) {
		//�A�C�e���x�[�X
		itemBase.Draw();
		//�A�C�e���@���C��
		for (int i = 0; i < 7; i++) {
			int num = i + (page - 1) * 7;
			if (num > 11) {
				break;
			}
			itemBaseLine[i].Draw();
		}
		for (int i = 0; i < 7; i++) {
			int num = i + (page - 1) * 7;
			if (num > 11) {
				break;
			}
			//�A�C�e�����O�@����
			float y = 340 + i * itemBaseLine[0].GetHeight();
			if (item->GetItemGet(num)) {
				meirio->DrawString(item->GetItemName(num), Vector2(1420, y), 1.8, Color(63, 41, 20, alpha), true);
			}
			else {
				meirio->DrawString("�H�H�H", Vector2(1420, y), 1.8, Color(63, 41, 20, alpha), true);
			}
			//�A�C�e���̏�����
			char tmp[256];
			sprintf_s(tmp, "%d", item->GetItemNum(num));
			meirio->DrawString(tmp, Vector2(1750, y), 1.8, Color(63, 41, 20, alpha), true);
		}

		//�A�C�e���I���t���[��
		selectFrame.Draw();
		//�y�[�W����{�^��
		rightArrow.Draw();
		leftArrow.Draw();
		//�y�[�W���\��������
		char tmp[50];
		sprintf_s(tmp, "%d Page", page);
		akazukin->DrawString(tmp, Vector2(1550, 960), 2.2, Color(1, 1, 1, alpha), true);
		
		
		//������
		for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
			manualBase[i].Draw();
			if (planterSetItem[i] == -1) {
				continue;
			}

			if (item->GetItemGet(planterSetItem[i])) {
				Vector2 pos = manualBase[i].GetPos();
				pos.Y -= 50;
				//�A�C�e�������@�A�C�e��
				items[i][planterSetItem[i]].SetAll(pos, Vector2(0.7, 0.7), 0, alpha);
				items[i][planterSetItem[i]].Draw();
				//�A�C�e������������
				Color manualStrColor(63.0f, 41.0f, 20.0f, alpha);
				//�A�C�e���������Q���O
				pos = manualBase[i].GetPos();
				pos.X -= 50;
				pos.Y -= 200;
				akazukin->DrawString(item->GetItemName(planterSetItem[i]), pos, 2.5 * 0.7, manualStrColor, true);
				//�A�C�e��������
				pos = manualBase[i].GetPos();
				pos.X -= 130;
				pos.Y += 50;
				akazukin->DrawString(item->GetManual1(planterSetItem[i]), pos, 1.5 * 0.7, manualStrColor);
				pos.Y += 50;
				akazukin->DrawString(item->GetManual2(planterSetItem[i]), pos, 1.5 * 0.7, manualStrColor);
				//�琬���s��
				pos = manualBase[i].GetPos();
				char tmp[50];
				sprintf_s(tmp, "�琬�������F%d",100-bugSurface[i]);
				pos.X -= 130;
				pos.Y += 150;
				akazukin->DrawString(tmp, pos, 1.5 * 0.7, manualStrColor);
				pos.Y += 30;
				sprintf_s(tmp, "���悯�F%d",bugFix[i]);
				akazukin->DrawString(tmp, pos, 1.5 * 0.7, manualStrColor);
				
			}

		}

	}

}
void PlanterSystem::Mating(int a, int b) {
	for (int i = 0; i < 3; i++) {
		if (matingSeed[i] != -1) {
			continue;
		}

		if (planterSetItem[a] == 0 &&
			planterSetItem[b] == 0) {
			matingSeed[i] = 7;
		}
		if (planterSetItem[a] == 1 &&
			planterSetItem[b] == 2) {
			matingSeed[i] = 8;
		}
		if (planterSetItem[a] == 4 &&
			planterSetItem[b] == 4) {
			matingSeed[i] = 9;
		}
		if (planterSetItem[a] == 3 &&
			planterSetItem[b] == 5) {
			matingSeed[i] = 10;
		}
		break;
	}
}
void PlanterSystem::SeedGrow() {

	auto item = Item::GetInstance();

	//���̂��₷���ƃ����_���̒l���ׂĐ��������݂𔻕�
	for (int i = 0; i < item->GetItemNum("�}�W�b�N�v�����^�["); i++) {
		if (planterSetItem[i] != -1) {
			int rand = GameMath::GetRand(1, 100);
			if (bug[i] - bugFixSum < rand) {
				growSeed[i] = true;
			}
		}
	}

	//��z����
	SeedMating();
	//��z���ʂ��A�C�e���ɔ��f������
	for (int i = 0; i < 3; i++) {
		if (matingSeed[i] == -1) {
			continue;
		}
		
		item->AddItem(item->GetItemName(matingSeed[i]), 1);
	}

	//�A�C�e�������炷����
	for (int i = 0; i < 3; i++) {
		if (planterSetItem[i] == -1 ) {
			continue;
		}
		item->AddItem(item->GetItemName(planterSetItem[i]), -1);
	}

	//�����𑝂₷
	GetMoney();
	item->AddMoney(100);
}
void PlanterSystem::GetMoney() {
	auto item = Item::GetInstance();

	for (int i = 0; i < 3; i++) {
		if (planterSetItem[i] == -1 || growSeed[i] == false) {
			continue;
		}
		item->AddMoney(item->GetSell(planterSetItem[i]));
	}
}
void PlanterSystem::SeedMating() {

	auto item = Item::GetInstance();

	if (item->GetItemNum("�}�W�b�N�v�����^�[") == 1) {
		//���˂ۂہA���ۂ�
		for (int h = 0; h < 2; h++) {
			for (int i = 0; i < 3; i++) {
				if (matingSeed[i] != -1) {
					continue;
				}
				if (planterSetItem[h] == 1) {
					matingSeed[i] = 1;
				}
				if (planterSetItem[h] == 8) {
					matingSeed[i] = 8;
				}
				break;
			}
		}
	}
	if (item->GetItemNum("�}�W�b�N�v�����^�[") == 2) {	
		Mating(0, 1);
		Mating(1, 0);
		//���˂ۂہA���ۂ�
		for (int h = 0; h < 2; h++) {
			for (int i = 0; i < 3; i++) {
				if (matingSeed[i] != -1) {
					continue;
				}
				if (planterSetItem[h] == 1) {
					matingSeed[i] = 1;
				}
				if (planterSetItem[h] == 8) {
					matingSeed[i] = 8;
				}
				break;
			}
		}
	}
	if (item->GetItemNum("�}�W�b�N�v�����^�[") == 3) {
		Mating(0, 1);
		Mating(0, 2);
		Mating(1, 2);
		//���˂ۂہA���ۂ�
		for (int h = 0; h < 3; h++) {
			for (int i = 0; i < 3; i++) {
				if (matingSeed[i] != -1) {
					continue;
				}
				//���˂ۂ�
				if (planterSetItem[h] == 1) {
					matingSeed[i] = 1;
				}
				//���ۂ�
				if (planterSetItem[h] == 8) {
					matingSeed[i] = 8;
				}
				break;
			}
		}
		//�t���E�̎�
		for (int i = 0; i < 3; i++) {
			if (matingSeed[i] != -1) {
				continue;
			}
			if (planterSetItem[0] == 6 &&
				planterSetItem[1] == 6 &&
				planterSetItem[2] == 6) {
				matingSeed[i] = 11;
			}
			break;
		}
	}

}
