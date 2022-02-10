#pragma once
#include "Application.h"
#include "Objects.h"
#include "Player.h"
#include "CameraWork.h"

using namespace frauEngine;

enum class ScenarioName {
	PROLOGUE,
	HANDYSHOP_JOIN,
	MAGICSHOP_JOIN,
	HANDY_EVENT1,
	HANDY_EVENT2,
	MAGIC_EVENT1,

	HAPPY_END,
	BAD_END,
	MAX
};
enum class ScenarioDataName {
	CHARA_NAME,
	SCENARIO_1,
	SCENARIO_2,
	SCENARIO_3,
	ECPRESSION_LEFT,
	ECPRESSION_RIGHT,
	EFFETC,
	MAX
};

class NovelSystem {
public:
	static NovelSystem* GetInstance() {
		static NovelSystem novelSystem;
		return &novelSystem;
	}

	void Initialize();
	void SetEnable(bool _enable=true, ScenarioName _scenario= ScenarioName::MAX);
	void Update();
	void Draw();

	bool GetEnable() { return enable; }
	float GetAlphaUI() { return alphaUI; };

	void EndNovel();
	bool GetEnd(ScenarioName _num) {
		return end[(int)_num];
	}
	void SetStart(ScenarioName _num,bool _end) {
		start[(int)_num] = _end;
	}
	void SetEnd(ScenarioName _num,bool _end) {
		end[(int)_num] = _end;
	}

	void Reset() {
		enable = false;
		count = 0;	
		alphaUI = 0.0f;

		for (int i = 0; i < (int)ScenarioName::MAX; i++) {
			start[i] = false;
			end[i] = false;
		}

		charaAlpha[0] = 0;
		charaAlpha[1] = 0;

		count = 0;
		alphaUI = 0.0f;	//�m�x��UI��A�l

		autoMode = false;
		autoCount = 0;

		
		scenario.num = 0;
		scenario.page = 1;
		scenario.leftCharaNumOld = -1;
		scenario.rightCharaNumOld = -1;
		scenario.leftCharaNum = -1;
		scenario.rightCharaNum = -1;
	}
private:
	void InitializeScenario();

	
	void TurnPage();
	void LoadPage();
	void ScenarioEnd();

	void DrawChara();
	void DrawUI();
	void DrawScenarioText();
private:
	Resource* resource;

	bool enable = false;

	
	//�V�i���I�e�L�X�g�֘A
	struct ScenarioData {

		std::unordered_map<std::string, int> ecpression;

		const string FRAU_NORMAL =		"�t���E�Q�ӂ�";
		const string FRAU_SMILE =		"�t���E�Q������";
		const string FRAU_SURPRISE =	"�t���E�Q�т�����";
		const string FRAU_TROUBLE =		"�t���E�Q���܂�";

		const string CHRISTA_NORMAL =	"�N���X�^�Q�ӂ�";
		const string CHRISTA_SMILE =	"�N���X�^�Q������";
		const string CHRISTA_SURPRISE = "�N���X�^�Q�т�����";
		const string CHRISTA_TROUBLE =	"�N���X�^�Q���܂�";

		const string HANDY_NORMAL =		"�n���f�B�Q�ӂ�";
		const string HANDY_SMILE =		"�n���f�B�Q������";
		const string HANDY_SURPRISE =	"�n���f�B�Q�т�����";
		const string HANDY_TROUBLE =	"�n���f�B�Q���܂�";


		CSV csv[(int)ScenarioName::MAX];
		int num = 0;
		int page = 1;

		int leftCharaNumOld = -1;
		int rightCharaNumOld = -1;
		int leftCharaNum  = -1;
		int rightCharaNum = -1;
		string str[(int)ScenarioDataName::MAX];

	};

	ScenarioData scenario;

	//�����G�֘A
	const float charaAlphaSpeed = 0.05f;
	float charaAlpha[2] = {0,0};
	const float charaPosDistX = 100;
	const float charaScale = 0.5f;
	const Vector2 charaPos[2] = { Vector2(960 - 600,800),Vector2(960 + 600,800) };
	ImageObject chara[12];

	//�m�x������UI�֘A
	int count = 0;			//�m�x�����n�܂��Ă���̃J�E���g
	const int countMax = 30;
	float alphaUI = 0.0f;	//�m�x��UI��A�l
	ImageObject textBase;
	ImageObject frame[2];

	bool autoMode = false;
	int autoCount = 0;
	int autoCountMax = 120;
	int autoSpeed = 6;

	ImageObject autoButton;
	ImageObject skipButton;

	bool start[(int)ScenarioName::MAX];
	bool end[(int)ScenarioName::MAX];
	
};