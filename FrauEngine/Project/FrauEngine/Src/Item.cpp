#include "Item.h"

void Item::Update() {
	

	//‚¨‹à‚Ì•`‰æ—p•¶Žš‚ÌŒvŽZ
	if (moneySurface != money) {
		//‚¨‹à‚Æ•`‰æ—p‚Ì‚¨‹à‚Ì·•ª‚Å‘¬“x‚ð•Ï‚¦‚é
		int moneySurfaceSpeed = 1;
		int moneyDistDigits = 0;
		int moneyDist = abs(money - moneySurface);///·‚ðo‚·
		moneyDistDigits = log10(moneyDist);//Œ…”-1‚ðo‚·
		if (moneyDistDigits > 1) {
			moneySurfaceSpeed *= pow(10, moneyDistDigits - 1);
		}
		//•`‰æ‹àŠz‚Ì‘Œ¸ˆ—
		if (moneySurface > money) {
			moneySurface -= moneySurfaceSpeed;
		}
		else if (moneySurface < money) {
			moneySurface += moneySurfaceSpeed;
		}
		//ŒvŽZ‚µ‚½‹àŠz‚ð•¶Žš—ñ‚ð‘ã“ü‚·‚é
		char moneySurfaceStr[50];
		sprintf_s(moneySurfaceStr, "%d", moneySurface);


		//Œ…”‚ª‘«‚è‚È‚¢æ“ª‚É‚Í0‚ð‹l‚ß‚é
		if (moneySurface != 0) {
			int moneyDigits = log10(moneySurface) + 1;//Œ…”‚ðo‚·		
			char digitStr[50] = "";
			for (int i = 0; i < maxMoneyDigits - moneyDigits; i++) {
				strcat_s(digitStr, "0");
			}
			strcat_s(digitStr, moneySurfaceStr);
			moneyStr = digitStr;
		}
		else {
			char digitStr[50] = "";
			for (int i = 0; i < maxMoneyDigits; i++) {
				strcat_s(digitStr, "0");
			}
			moneyStr = digitStr;
		}
	}

}