#include "GameManager.h"

namespace CleditVariable {

	int count = 0;
	int speed = 5;
	int countMax = 1080 * 7*2-1080;

	int stopCount = 0;
	int stopCountMax = 180;

}
using namespace CleditVariable;

void GameManager::CleditLoad() {
	count = 0;
	gs->BGM[11].Play(true);
}
void GameManager::CleditUpdata() {

	if (count < countMax) {
		if (stopCount == 0) {
			count += speed;
		}
		if (count % 1080 == 0) {
			stopCount++;
			if (stopCount > stopCountMax) {
				stopCount = 0;
			}
		}
	}
	if (MOUSE->right >= 1) {
		credit = true;
		gs->Enter.Play();
		gs->BGM[11].Stop();
		scene = SELECT;
	}
}
void GameManager::CleditDraw() {

	gi->cleditBack.Draw(960, 540);
	for (int i = 0; i < 7; i++) {
		gi->cleditPhoto[i].Draw(1400, 540 + 1080*2 * i + 1080 - count);
	}

}
