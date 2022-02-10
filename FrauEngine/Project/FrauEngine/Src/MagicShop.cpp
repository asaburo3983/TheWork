#include "MagicShop.h"
#include "SoundManager.h"
void MagicShop::Initialize(std::string _itemCSVPath, std::string _textCSVPath) {

	auto rc = Resource::GetInstance();
	npc[0].SetResource(rc->Image("Christa_Normal.png"));
	npc[1].SetResource(rc->Image("Christa_Smile.png"));
	npc[2].SetResource(rc->Image("Christa_Surprise.png"));
	npc[3].SetResource(rc->Image("Christa_Trouble.png"));
	for (int i = 0; i < 4; i++) {
		npc[i].SetAll(Vector2(400, 760), Vector2(0.5, 0.5), 0, alpha);
		npc[i].SetTurn(true, false);
	}
	Shop::Initialize(_itemCSVPath, _textCSVPath);

	itemIcon[0].SetResource(rc->Image("MagicPlanter.png"));
	itemIcon[1].SetResource(rc->Image("BugFix.png"));
	itemIcon[2].SetResource(rc->Image("Book0.png"));
	itemIcon[3].SetResource(rc->Image("Book1.png"));
	itemIcon[4].SetResource(rc->Image("Book2.png"));
	itemIcon[5].SetResource(rc->Image("Book3.png"));
	itemIcon[6].SetResource(rc->Image("Book4.png"));

	
}
void MagicShop::SelectItem() {
	auto mouse = MouseInput::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto item = Item::GetInstance();

	if (mouse->left == 1) {
		for (int i = 0; i < itemValueNum; i++) {
			if (shopBaseLine[i].Hit(mouse->x, mouse->y)) {
				int oldSelect = selectNum;
				selectNum = i;
				//��萔�ȏ�w���ł��Ȃ��悤�ɂ���
				if (selectNum == 0|| selectNum == 1) {
					if (item->GetItemNum(12 + i) >= 3) {
						selectNum = oldSelect;
						return;
					}
				}
				else if (selectNum > 1) {
					if (item->GetItemNum(i + 12) >= 1) {
						selectNum = oldSelect;
						return;
					}
				}
				buyNum = 1;
				state = State::SELECT;
				sound->GetSE(SoundList_SE::ENTER)->Play();
				break;
			}
		}
	}
}
void MagicShop::Buy() {
	auto mouse = MouseInput::GetInstance();
	auto item = Item::GetInstance();
	auto sound = SoundManager::GetInstance();
	if (mouse->left == 1) {
		if (buyButton.Hit(mouse->x, mouse->y)) {
			if (item->GetMoney() < allPrice) {
				//����������Ȃ�
				state = State::NOT_MONEY;
			}
			else {
				//�w������
				item->AddItem(itemName[selectNum], buyNum);
				item->AddMoney(-allPrice);
				state = State::BUY;
				sound->GetSE(SoundList_SE::BUY)->Play();
				selectNum = -1;
			}

		}
	}
}
void MagicShop::Update() {
	auto item = Item::GetInstance();
	auto cameraWork = CameraWork::GetInstance();
	
	if (enable) {
		
		Shop::Cancel();
		//�v���C���[�̓��͂ɑ΂��锽��
		switch (state) {
		case State::NORMAL:
			charaEcpression = (int)Ecpression::NORMAL;
			SelectItem();
			break;
		case State::SELECT:
			if (manualAlpha < 1.0f) {
				manualAlpha += alphaSpeed;
			}
			charaEcpression = (int)Ecpression::NORMAL;
			SelectItem();
			//�v�����^�[�̍w������ڂ̎����z���グ��
			if (selectNum == 0&& item->GetItemNum(12) == 2) {
				allPrice = 100000;
			}
			//�o�O�t�B�b�N�X�����ŋ��z���グ��
			else if (selectNum == 1 && item->GetItemNum(13) >= 1) {
				allPrice = 50000 * item->GetItemNum(13);
			}
			else {
				allPrice = itemParam[selectNum].price;
			}
			Buy();
			break;
		case State::BUY:
			charaEcpression = (int)Ecpression::SMILE;
			break;
		case State::BYE:
			charaEcpression = (int)Ecpression::SMILE;
			break;
		case State::NOT_MONEY:
			charaEcpression = (int)Ecpression::TROUBLE;
			break;
		}
		if (state != State::SELECT) {
			if (manualAlpha > 0.0f) {
				manualAlpha -= alphaSpeed;
			}
		}

		if (alpha < 1) {
			alpha += alphaSpeed;
		}

	}
	else {
		if (cameraWork->GetMoveNum() == 3) {
			cameraWork->SetMoveNum(0);
		}

		if (alpha > 0) {
			alpha -= alphaSpeed;
		}
		if (manualAlpha > 0.0f) {
			manualAlpha -= alphaSpeed;
		}
	}
	//�`�悷��A�l��ݒ�
	shopBase.SetAlpha(alpha);
	for (int i = 0; i < itemValueNum; i++) {
		shopBaseLine[i].SetAlpha(alpha);
	}
	manualBase.SetAlpha(manualAlpha);

	textBase.SetAlpha(alpha);
	rightArrow.SetAlpha(alpha);
	leftArrow.SetAlpha(alpha);
	strColor.A = alpha;
	buyButton.SetAlpha(alpha);
}
void MagicShop::Draw() {

	//�����G�`��
	npc[charaEcpression].SetAlpha(alpha);
	npc[charaEcpression].Draw();

	Font* meirio = nullptr;
	meirio = Fonts::GetInstance()->GetFont((int)FontList::MEIRIO);
	Font* akazukin = nullptr;
	akazukin = Fonts::GetInstance()->GetFont((int)FontList::AKAZUKIN);

	auto item = Item::GetInstance();
	//�A�C�e�����X�g�x�[�X
	shopBase.Draw();
	for (int i = 0; i < itemValueNum; i++) {
		if (i == 0||i==1) {
			if (item->GetItemNum(12+i) >= 3) {
				strColor.A = 0.5f;
			}
			else {
				if (enable) {
					strColor.A = 1.0f;
				}
			}
		}
		else {
			if (item->GetItemNum(12+i) >= 1) {
				strColor.A = 0.5f;
			}
			else {
				if (enable) {
					strColor.A = 1.0f;
				}
			}
		}
		strColor.A *= alpha;
		//�A�C�e������
		shopBaseLine[i].Draw();
		//�A�C�e����
		float y = 340 + i * shopBaseLine[i].GetHeight();
		float scale = 1.8;

		meirio->DrawString(itemName[i], Vector2(1420, y), scale, strColor, true);

		char tmp[256];
		//�A�C�e���̏�����
		sprintf_s(tmp, "%d", item->GetItemNum(itemName[i]));
		meirio->DrawString(tmp, Vector2(1650, y), scale, strColor, true);

		//�A�C�e���̂P������̋��z
		if (item->GetItemNum(12) == 2 && i == 0) {
			sprintf_s(tmp, "%d", 100000);
			meirio->DrawString(tmp, Vector2(1750, y), scale, strColor, true);
		}
		else if (item->GetItemNum(13) >= 1 && i == 1) {
			sprintf_s(tmp, "%d", 50000* item->GetItemNum(13));
			meirio->DrawString(tmp, Vector2(1750, y), scale, strColor, true);
		}
		else {
			sprintf_s(tmp, "%d", itemParam[i].price);
			meirio->DrawString(tmp, Vector2(1750, y), scale, strColor, true);
		}
	}
	selectFrame.SetAll(Vector2(1575, 340 + selectNum * shopBaseLine[0].GetHeight()), Vector2(1, 1), 0, alpha);
	selectFrame.Draw();

	//������̃x�[�X
	textBase.Draw();
	//�L�����̖��O
	akazukin->DrawString(reactionParam[(int)state].name, Vector2(330, 800), 2.5, strColor, true);
	//�L�����̃e�L�X�g
	meirio->DrawString(reactionParam[(int)state].str1, strPos, strFloat, strColor);

	//�A�C�e���������x�[�X
	manualBase.Draw();
	if (selectNum != -1) {
		Color manualStrColor = strColor;
		manualStrColor.A = manualAlpha;
		//�A�C�e���������Q���O
		akazukin->DrawString(itemName[selectNum], Vector2(1000, 140), 2.5, manualStrColor, true);
		//�A�C�e��������
		akazukin->DrawString(itemParam[selectNum].str1, Vector2(810, 500), 1.5, manualStrColor);
		akazukin->DrawString(itemParam[selectNum].str2, Vector2(810, 570), 1.5, manualStrColor);
	}
	switch (state) {
	case State::SELECT:

		//�w����
		char tmp[256];
		sprintf_s(tmp, "�w�����@%d��", buyNum);
		meirio->DrawString(tmp, Vector2(330, 945), 2, strColor);
		//�w�����z
		sprintf_s(tmp, "���v���z�@%d���E", allPrice);
		meirio->DrawString(tmp, Vector2(580, 945), 2, strColor);

		//�w���{�^��
		buyButton.Draw();

		break;
	}

	if (selectNum != -1) {
		itemIcon[selectNum].SetAll(Vector2(1015, 320), Vector2(1, 1), 0, manualAlpha);
		itemIcon[selectNum].Draw();
	}

}