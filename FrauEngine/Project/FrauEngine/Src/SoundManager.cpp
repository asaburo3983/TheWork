#include "SoundManager.h"

void SoundManager::Initialize() {
	auto rc = Resource::GetInstance();
	
	//BGM
	bgm[0].SetResource(rc->Sound("Title_BGM.mp3"));
	bgm[1].SetResource(rc->Sound("FlowerShop_BGM.mp3"));
	bgm[2].SetResource(rc->Sound("Map_BGM.mp3"));
	bgm[3].SetResource(rc->Sound("HandyShop_BGM.mp3"));
	bgm[4].SetResource(rc->Sound("MagicShop_BGM.mp3"));
	bgm[5].SetResource(rc->Sound("HappyEnd_BGM.mp3"));
	bgm[6].SetResource(rc->Sound("BadEnd_BGM.mp3"));
	bgm[7].SetResource(rc->Sound("ShindyTheme_BGM.mp3"));
	bgm[8].SetResource(rc->Sound("HandyTheme_BGM.mp3"));
	bgm[9].SetResource(rc->Sound("Endroll_BGM.mp3"));

	for (int i = 0; i < (int)SoundList_BGM::MAX; i++) {
		bgm[i].SetLoop(true);
	}
	SetVolumeBGM(bgmVolume);
	SetVolumeBGMMulti(bgmVolumeMulti);

	//SE
	se[0].SetResource(rc->Sound("Buy_SE.mp3"));
	se[1].SetResource(rc->Sound("Cancel_SE.mp3"));
	se[2].SetResource(rc->Sound("CursorMove_SE.mp3"));
	se[3].SetResource(rc->Sound("Door_SE.mp3"));
	se[4].SetResource(rc->Sound("Enter_SE.mp3"));
	se[5].SetResource(rc->Sound("Morning_SE.mp3"));
	se[6].SetResource(rc->Sound("Walk_SE.mp3"));
	se[7].SetResource(rc->Sound("NextDay_Jingle.mp3"));
	se[8].SetResource(rc->Sound("TurnPage_SE.mp3"));

	SetVolumeSE(seVolume);
	SetVolumeSEMulti(seVolumeMulti);

}
void SoundManager::SetVolumeBGM(float _volume) {
	bgmVolume = _volume;
	for (int i = 0; i < (int)SoundList_BGM::MAX; i++) {
		bgm[i].SetVolume(bgmVolume);
	}
}
void SoundManager::SetVolumeSE(float _volume) {
	seVolume = _volume;
	for (int i = 0; i < (int)SoundList_SE::MAX; i++) {
		se[i].SetVolume(seVolume);
	}
}
void SoundManager::SetVolumeBGMMulti(float _volume) {
	bgmVolumeMulti = _volume;
	for (int i = 0; i < (int)SoundList_BGM::MAX; i++) {
		bgm[i].SetVolumeMulti(bgmVolumeMulti);
	}
}
void SoundManager::SetVolumeSEMulti(float _volume) {
	seVolumeMulti = _volume;
	for (int i = 0; i < (int)SoundList_SE::MAX; i++) {
		se[i].SetVolumeMulti(seVolumeMulti);
	}
}