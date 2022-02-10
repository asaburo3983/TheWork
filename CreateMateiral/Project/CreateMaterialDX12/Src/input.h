//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#pragma comment (lib, "xinput.lib")
#include <xinput.h>

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);
long GetMouseX(void);
long GetMouseY(void);

class KeyInput {
public:
	static KeyInput* GetInstance() {
		static KeyInput keyInput;
		return &keyInput;
	}
public:
	//�L�[�{�[�h�n
	int key[256];
	// �L�[�̓��͏�Ԃ��X�V����
	void updata() {
		for (int i = 0; i < 256; i++) {
			if (GetKeyboardPress(i)==true)
				key[i]++;
			else
				key[i] = 0;
		}
	}
};
class MouseInput {
public:
	static MouseInput* GetInstance() {
		static MouseInput mouseInput;
		return &mouseInput;
	}
public:
	long x, y;
	long axisX, axisY, axisZ;
	int left,right,center;
	void updata() {
		if (GetMouseLeftPress())
			left++;
		else
			left = 0;
		if (GetMouseRightPress())
			right++;
		else
			right = 0;
		if (GetMouseCenterPress())
			center++;
		else
			center = 0;

		axisX = GetMouseAxisX();
		axisY = GetMouseAxisY();
		axisZ = GetMouseAxisZ();

		x = GetMouseX();
		y = GetMouseY();
	}
};


enum X_BUTTON {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	L1,
	R1,
	A,
	B,
	X,
	Y,
};
class X_INPUT {
//�g�p���@�@�錾����updata�����[�v������
private:
	XINPUT_STATE state;
	int num;
	XINPUT_VIBRATION vibration;
public://�{�^���ƃX�e�B�b�N�̏��
	int button[14];
	int stickLX, stickLY, stickRX, stickRY;
public:
	X_INPUT() {}
	X_INPUT(int _num) {
		num = _num; 
		for (int i = 0; i < 14; i++) {
			button[i] = 0;
		}
	};
	void updata() {
		XInputGetState(num, &state);

		int i = 0x0001;
		for (int h = 0; h < 14; h++) {
			if (state.Gamepad.wButtons & i) {
				if(button[h]<10000)
				button[h]++;				
			}
			else
				button[h] = 0;

			if (i == 0x0200)
				i = i << 2;

			i = i << 1;
		}

		stickLX = state.Gamepad.sThumbLX;
		stickLY = state.Gamepad.sThumbLY;
		stickRX = state.Gamepad.sThumbRX;
		stickRY = state.Gamepad.sThumbRY;

	}
	void SetVibSpeed(int _speed) {
		vibration.wRightMotorSpeed = _speed;
		vibration.wLeftMotorSpeed = _speed;
		XInputSetState(num, &vibration);
	}

};
#endif