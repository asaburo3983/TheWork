#include "Item.h"

void Item::Update() {
	

	//�����̕`��p�����̌v�Z
	if (moneySurface != money) {
		//�����ƕ`��p�̂����̍����ő��x��ς���
		int moneySurfaceSpeed = 1;
		int moneyDistDigits = 0;
		int moneyDist = abs(money - moneySurface);///�����o��
		moneyDistDigits = log10(moneyDist);//����-1���o��
		if (moneyDistDigits > 1) {
			moneySurfaceSpeed *= pow(10, moneyDistDigits - 1);
		}
		//�`����z�̑�������
		if (moneySurface > money) {
			moneySurface -= moneySurfaceSpeed;
		}
		else if (moneySurface < money) {
			moneySurface += moneySurfaceSpeed;
		}
		//�v�Z�������z�𕶎����������
		char moneySurfaceStr[50];
		sprintf_s(moneySurfaceStr, "%d", moneySurface);


		//����������Ȃ��擪�ɂ�0���l�߂�
		if (moneySurface != 0) {
			int moneyDigits = log10(moneySurface) + 1;//�������o��		
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