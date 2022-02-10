#include "EventManager.h"
#include "SoundManager.h"


void EventManager::Initialize() {
	isStartDays = true;
	days = 1;
	fade = false;
	fadeSpeed = 0.05f;
	fadeCount = 0.0f;
}
void EventManager::StartDays() {
	
	if (!isStartDays) {
		return;
	}
	//���ɂ����ς�����������ɔ�������C�x���g
	Player* player = Player::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();
	auto item = Item::GetInstance();
	auto sound = SoundManager::GetInstance();

	switch (days) {
	case 1:
		player->SetPos(Vector3(4.5, -3.9, 5.2));
		player->SetAngle(Vector3(0, 180, 0));
		novelSystem->SetEnable(true, ScenarioName::PROLOGUE);
		camera->SetMoveNum(1);
		item->AddItem("�}�W�b�N�v�����^�[", 1);//�ǉ�
		item->AddItem("�}�\�n�[�u�̎�", 3);

		//�f�o�b�O�p�A�C�e���̒ǉ�
		if (LowApplication::GetInstance()->GetDebugMode()) {
			item->AddItem("�}�W�b�N�v�����^�[", 2);//�ǉ�
			item->AddItem("�}�\�n�[�u�̎�", 10);//�ǉ�
			item->AddItem("�^�l�|�|�̎�", 10);//�ǉ�
			item->AddItem("�x�����_�[�̎�", 10);//�ǉ�
			item->AddItem("�p���_�W�[�̎�", 10);//�ǉ�
			item->AddItem("�o�����o�̎�", 10);//�ǉ�
			item->AddItem("�V���K�[�X�e���̎�", 10);//�ǉ�
			item->AddItem("�R�X���̎�", 10);//�ǉ�
		}

		break;
	case 2:
		break;
	case 4:
		if (novelSystem->GetEnd(ScenarioName::HANDYSHOP_JOIN) &&
			novelSystem->GetEnd(ScenarioName::MAGICSHOP_JOIN)) {
			novelSystem->SetEnable(true, ScenarioName::HANDY_EVENT1);
		}
		break;
	case 11:
		if (novelSystem->GetEnd(ScenarioName::HANDY_EVENT1)) {
			novelSystem->SetEnable(true, ScenarioName::MAGIC_EVENT1);
		}
		break;
	case 17:
		if (novelSystem->GetEnd(ScenarioName::MAGIC_EVENT1)) {
			novelSystem->SetEnable(true, ScenarioName::HANDY_EVENT2);
		}
		break;
	case 31:
		//100�����E�Ȃ���΃Q�[���I�[�o�[
		if (item->GetMoney() < 1000000) {
			novelSystem->SetEnable(true, ScenarioName::BAD_END);
		}
		break;
	}
	//100�����E���܂�΂��̎��_�ŃN���A
	if (item->GetMoney() >= 1000000) {
		novelSystem->SetEnable(true, ScenarioName::HAPPY_END);
	}
	isStartDays = false;
}
void EventManager::AddDays() {
	days++;
}
void EventManager::EndDays() {
	isStartDays = true;
}
void EventManager::FieldEvent() {
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();
	auto mouse = MouseInput::GetInstance();
	auto handyShop = HandyShop::GetInstance();
	auto magicShop = MagicShop::GetInstance();
	auto palnterSystem = PlanterSystem::GetInstance();
	auto nextDay = NextDay::GetInstance();
	auto sound = SoundManager::GetInstance();
	auto novelSystem = NovelSystem::GetInstance();
	CameraWork* camera = CameraWork::GetInstance();

	int eventNum = player->GetEventNum();

	//�C�x���g���͏���
	if (mouse->left == 1 && eventNum != -1) {
		
		switch (eventNum) {
		//�ړ��C�x���g�p�̃t�F�[�h
		case (int)EventNum::EXIT:
		case (int)EventNum::JOIN_FLOWERSHOP:
		case (int)EventNum::JOIN_HANDYSHOP:
		case (int)EventNum::JOIN_MAGICSHOP:
			fade = true;
			sound->GetSE(SoundList_SE::DOOR)->Play();
			break;
		case (int)EventNum::PLANTER:
			palnterSystem->SetEnable(true);
			
			break;
		case (int)EventNum::OPEN_SHOP:
			//���t���܂��������A�m�F�����
			if (nextDay->GetProcessEnable() == false && novelSystem->GetEnable()==false) {
				nextDay->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
			}
			break;
		case (int)EventNum::BUY_HANDYSHOP:
			if (handyShop->GetEnable() == false) {
				handyShop->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
				camera->SetMoveNum(2);
			}
			break;
		case (int)EventNum::BUY_MAGICSHOP:
			if (magicShop->GetEnable() == false) {
				magicShop->SetEnable(true);
				sound->GetSE(SoundList_SE::ENTER)->Play();
				camera->SetMoveNum(3);
			}
			break;
		}
	}
	
	//�t�F�[�h����
	if (fade) {
		if (fadeCount <1.0f) {
			fadeCount += fadeSpeed;
		}
		player->IsMove(false);
	}
	else {
		if (fadeCount > 0) {
			fadeCount -= fadeSpeed; 
		}
	}
	//�t�F�[�h���^���ÂȎ��̏���
	if (fadeCount >= 1.0f) {
		fade = false;
		player->IsMove(true);
		switch (eventNum) {
			//����̓V�[���ړ��̏����ɂ��Ă��܂�
		case (int)EventNum::EXIT:
			
			scene->LoadScene("Map");
			stage->MoveStage((int)StageNum::MAP);

		

			break;
		case (int)EventNum::JOIN_FLOWERSHOP:
			
			scene->LoadScene("FlowerShop");
			stage->MoveStage((int)StageNum::FLOWER_SHOP);
			break;
		case (int)EventNum::JOIN_HANDYSHOP:
			scene->LoadScene("HandyShop");
			stage->MoveStage((int)StageNum::HANDY_SHOP);

			
			break;
		case (int)EventNum::JOIN_MAGICSHOP:
			scene->LoadScene("MagicShop");
			stage->MoveStage((int)StageNum::MAGIC_SHOP);

			
			break;
		}
	}

}
void EventManager::JoinTutorial() {
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	Player* player = Player::GetInstance();
	Stage* stage = Stage::GetInstance();

	//�}�b�v�ړ��������Ƃ��ɕ\������`���[�g���A��
	switch (stage->GetStageNum()) {
	case (int)StageNum::FLOWER_SHOP:
		//������@�̃`���[�g���A��
		tutorialSystem->SetEnable(true, 0);

		//�v�����^�[�̎g�����`���[�g���A��
		if (tutorialSystem->GetEnd(1)) {
			tutorialSystem->SetEnable(true, 2);
		}
		//�J�E���^�[�̎g�����`���[�g���A��
		if (tutorialSystem->GetEnd(2)) {
			tutorialSystem->SetEnable(true, 3);
		}
		break;
	case (int)StageNum::MAP:
		break;
	case (int)StageNum::HANDY_SHOP:
		novelSystem->SetEnable(true, ScenarioName::HANDYSHOP_JOIN);
		tutorialSystem->SetEnable(true, 1);
		
		break;
	case (int)StageNum::MAGIC_SHOP:
		novelSystem->SetEnable(true, ScenarioName::MAGICSHOP_JOIN);
		tutorialSystem->SetEnable(true, 1);
		
		break;
	}

}
void EventManager::GameEndEvent() {
	NovelSystem* novelSystem = NovelSystem::GetInstance();

	//�Ƃ肠�����ȑf��scene�ړ�����
	if (novelSystem->GetEnd(ScenarioName::BAD_END) || novelSystem->GetEnd(ScenarioName::HAPPY_END)) {
		scene->LoadScene("Endroll");
	}
}
void EventManager::Update() {
	CameraWork* camera = CameraWork::GetInstance();
	NovelSystem* novelSystem = NovelSystem::GetInstance();
	TutorialSystem* tutorialSystem = TutorialSystem::GetInstance();
	Stage* stage = Stage::GetInstance();
	auto key = KeyInput::GetInstance();

	StartDays();

	JoinTutorial();

	FieldEvent();

	GameEndEvent();
	//�}�b�v�̎��̂݃J�����͒Ǐ]������
	if (stage->GetStageNum() == (int)StageNum::MAP) {
		camera->SetPlayerTarget(true);
	}
	else {
		camera->SetPlayerTarget(false);
	}

	//�I�v�V������ʂւ̈ڍs
	//TODO ���y���~�߂鏈�����ǉ����Ȃ���΂Ȃ�Ȃ�
	if (key->key[DIK_ESCAPE] == 1) {
		scene->LoadScene("Option");
	}
}