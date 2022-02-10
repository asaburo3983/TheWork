#pragma once
#include "Application.h"
#include "Objects.h"

using namespace frauEngine;

enum class SoundList_BGM {
	TITLE,
	FLOWER_SHOP,
	MAP,
	HANDY_SHOP,
	MAGIC_SHOP,
	HAPPY_END,
	BAD_END,
	SHINDY_THEME,
	HANDY_THEME,
	ENDROLL,
	MAX
};
enum class SoundList_SE {
	BUY,
	CANCEL,
	CURSOR_MOVE,
	DOOR,
	ENTER,
	MORNING,
	WALK,
	NEXTDAY,
	TURNPAGE,
	MAX
};

class SoundManager {
public:
	static SoundManager* GetInstance() {
		static SoundManager sm;
		return &sm;
	}
private:
	SoundObject se[(int)SoundList_SE::MAX];
	SoundObject bgm[(int)SoundList_BGM::MAX];

	float seVolume = 0.5f;
	float bgmVolume = 0.5f;
	float seVolumeMulti = 1.0f;
	float bgmVolumeMulti = 1.0f;

	float fadeSpeed = 0.05f;
public:
	void Initialize();
	void SetVolumeAll(float _volume){
		SetVolumeBGM(_volume);
		SetVolumeSE(_volume);
	}
	void SetVolumeBGM(float _volume);
	void SetVolumeSE(float _volume);
	void SetVolumeAllMulti(float _volume){
		SetVolumeBGMMulti(_volume);
		SetVolumeSEMulti(_volume);
	}
	void SetVolumeBGMMulti(float _volume);
	void SetVolumeSEMulti(float _volume);

	SoundObject* GetBGM(SoundList_BGM _bgm) {
		return &bgm[(int)_bgm];
	}
	SoundObject* GetSE(SoundList_SE _se) {
		return &se[(int)_se];
	}

	void PlayFade(SoundList_BGM _bgm) {
		if (bgm[(int)_bgm].GetVolumeMulti() < 1.0f) {
			bgm[(int)_bgm].AddVolumeMulti(fadeSpeed);
		}
		bgm[(int)_bgm].Play();
	}
	void PlayFade(SoundList_SE _se) {
		if (se[(int)_se].GetVolumeMulti() < 1.0f) {
			se[(int)_se].AddVolumeMulti(fadeSpeed);
		}
		se[(int)_se].Play();
	}

	void StopFade(SoundList_BGM _bgm) {
		if (bgm[(int)_bgm].GetVolumeMulti() > 0.0f) {
			bgm[(int)_bgm].AddVolumeMulti(-fadeSpeed);
			bgm[(int)_bgm].Play();
		}
		else {
			bgm[(int)_bgm].Stop();
		}	
	}
	void StopFade(SoundList_SE _se) {
		if (se[(int)_se].GetVolumeMulti() > 0.0f) {
			se[(int)_se].AddVolumeMulti(-fadeSpeed);
			se[(int)_se].Play();
		}
		else {
			se[(int)_se].Stop();
		}
	}

};