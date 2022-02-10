#pragma once
#include "Application.h"
#include "Objects.h"
#include "EventManager.h"
#include "PlanterSystem.h"
#include "Save.h"

using namespace frauEngine;

class NextDay {
private:
	

	const std::string str = "今日の準備を終えて次の日に進みますか？";

	ImageObject base;
	ImageObject yes;
	ImageObject no;
	ImageObject morningBase;

	ImageObject seed[6][3];

	bool processEnable = false;
	bool enable = false;
	float fade = 0.0f;
	const float fadeSpeed = 0.05f;

	bool night = false;
	float nightAlpha = 0.0f;
	float nightAlphaMax = 0.1f;/* 7.0f;*/
	bool morning = false;
	float morningAlpha = 0.0f;
public:
	static NextDay* GetInstance() {
		static NextDay ev;
		return &ev;
	}

	void SetEnable(bool _enable) { 
		auto mouse = MouseInput::GetInstance();
		auto player = Player::GetInstance();
		enable = _enable; 
		if (enable) {
			processEnable = true;		
		}	
		player->IsMove(!enable);
		//多重クリックの阻止のためにフレーム数を増加
		mouse->left = 2;
	}
	void SetMorning(bool _enable) { morning = _enable; }
	bool GetNight() { return night; }
	bool GetMorning() { return morning; }

	void Initialize();
	void Update();
	void Draw();
	void DrawCheck();
	void DrawMorning();
	float GetFade() { return nightAlpha; }
	bool GetProcessEnable() { return processEnable; }
};