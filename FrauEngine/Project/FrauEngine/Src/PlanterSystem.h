#pragma once
#include "Application.h"
#include "Objects.h"
#include "Item.h"
#include "EventManager.h"

using namespace frauEngine;

class PlanterSystem {
public:
	static PlanterSystem* GetInstance() {
		static PlanterSystem n;
		return &n;
	}
	void Reset();
	void SetEnable(bool _enable);
	void Initialize();
	void Update();
	void Draw();
	bool GetEnable() { return enable; }
	int GetSetItem(int _num) {
		return planterSetItem[_num];
	}

	void SeedGrow();//¬’·”»’è

	bool GetGrouSeedEnable(int _num) {
		return growSeed[_num];
	}
	int GetMatingSeed(int _num) {
		return matingSeed[_num];
	}

private:
	void Mating(int a,int b);
	void SeedMating();//Œğ”z”»’è
	void GetMoney();
private:

	bool enable = false;
	float alpha = 0.0f;
	const float alphaSpeed = 0.05f;
	int selectItemNum = 0;
	int page = 1;

	ImageObject itemBase;
	ImageObject itemBaseLine[7];
	ImageObject selectFrame;
	ImageObject rightArrow;
	ImageObject leftArrow;

	ImageObject manualBase[3];
	ImageObject items[3][12];

	int planterSetItem[3] = { -1,-1,-1 };

	int bug[3] = { 0,0,0 };//ÀÛ‚Ìˆç¬¸”s—¦
	int bugFixSum = 0;//ˆç¬¸”s—¦–h~i‡Œvj
	int bugFix[3] = { 0,0,0 };//ˆç¬¸”s—¦–h~iŒÂ•Êj
	int bugSurface[3] = { 0,0,0 };//ˆç¬¸”s—¦i•`‰æ—pj

	//ˆç¬Œ‹‰Ê
	bool growSeed[3] = { false,false,false };
	int matingSeed[3] = { -1,-1,-1 };

}; 