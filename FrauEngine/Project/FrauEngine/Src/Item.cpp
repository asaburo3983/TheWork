#include "Item.h"

void Item::Update() {
	

	//お金の描画用文字の計算
	if (moneySurface != money) {
		//お金と描画用のお金の差分で速度を変える
		int moneySurfaceSpeed = 1;
		int moneyDistDigits = 0;
		int moneyDist = abs(money - moneySurface);///差を出す
		moneyDistDigits = log10(moneyDist);//桁数-1を出す
		if (moneyDistDigits > 1) {
			moneySurfaceSpeed *= pow(10, moneyDistDigits - 1);
		}
		//描画金額の増減処理
		if (moneySurface > money) {
			moneySurface -= moneySurfaceSpeed;
		}
		else if (moneySurface < money) {
			moneySurface += moneySurfaceSpeed;
		}
		//計算した金額を文字列を代入する
		char moneySurfaceStr[50];
		sprintf_s(moneySurfaceStr, "%d", moneySurface);


		//桁数が足りない先頭には0を詰める
		if (moneySurface != 0) {
			int moneyDigits = log10(moneySurface) + 1;//桁数を出す		
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