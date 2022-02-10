#pragma once
#include "Application.h"
#include "Objects.h"
#include "Item.h"
#include "Player.h"
#include "CameraWork.h"

using namespace frauEngine;

class Shop {
protected:
	//���̃N���X�̗L��
	bool enable = false;
	//���
	enum class State {
		NORMAL,
		SELECT,
		BUY,
		NOT_MONEY,
		BYE
	};
	enum class Ecpression {
		NORMAL,
		SMILE,
		SURPRISE,
		TROUBLE
	};
	State state = State::NORMAL;

	//�A�C�e�����X�g
	int itemValueNum = 0;
	struct ItemParam {
		int price = 0;
		std::string str1;
		std::string str2;
	};
	std::vector<std::string> itemName;
	std::vector<ItemParam> itemParam;
	//��ʂɂ����郊�A�N�V����
	struct ReactionParam {
		std::string name;
		std::string str1;
	};
	std::vector<ReactionParam > reactionParam;

	//���\�[�X
	ImageObject shopBase;
	ImageObject shopBaseLine[7];
	ImageObject manualBase;
	ImageObject selectFrame;

	ImageObject textBase;
	ImageObject rightArrow;
	ImageObject leftArrow;
	ImageObject buyButton;

	CSV csvItem;
	CSV csvText;

	//ON_OFF�Ŏg��
	const float alphaSpeed = 0.05;
	float alpha = 0.0f;
	//�A�C�e���̑I��
	int selectNum = 0;
	//�w�����ƍ��v���z
	int buyNum = 0;
	int allPrice = 0;

	//�e�L�X�g�̌Œ�l
	const Vector2 strPos = Vector2(240, 880);
	const float strFloat = 2.0f;
	Color strColor = Color(63.0f / 256.0f, 41.0f / 256.0f, 20.0f / 256.0f, 1);

	//�����G�̎��
	int charaEcpression = 0;

	float manualAlpha = 0.0f;
public:
	void Initialize(std::string _itemCSVPath,std::string _textCSVPath);
	void SetEnable(bool _enable);
	void Update();
	void Draw();

	int GetBuyNum() { return buyNum; }
	int GetAllPrice() { return allPrice; }
	bool GetEnable() { return enable; }
	float GetAlpha() { return alpha; }
protected:
	void Cancel();
	void SelectItem();
	void SelectBuyNum();
	void Buy();
};
