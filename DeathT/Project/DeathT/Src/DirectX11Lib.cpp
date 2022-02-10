#include "DirectX11Lib.h"

void AllInit(const char* winName,int sizeX,int sizeY,bool fullScreen,HINSTANCE hInstance) {

	Window window(winName, sizeX, sizeY);
	// ƒEƒBƒ“ƒhƒE‚ğì¬
	window.Create(fullScreen);

	//ƒL[“ü—Í‰Šú‰»
	InitInput(hInstance, FindWindow(Window::ClassName, nullptr));

	DirectGraphics::GetInstance()->Init();


	DXTK11LIB::InitDXTK11Lib();
}
void SetAntialias(bool on) {
	DirectGraphics::GetInstance()->SetAntialias(on);
}
FBX_MODEL::FBX_MODEL() {
	con = DirectGraphics::GetInstance()->GetContext();
	pos = Vector3(0.0f, 0.0f, 0.0f);
	degree = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);

	axis = Vector3(1.0f, 1.0f, 1.0f);
	axisAngle = 0;
}
void FBX_MODEL::SetScaleALL(float _scale) {
	scale = Vector3(_scale, _scale, _scale);
}
void FBX_MODEL::RenderShadowMap(MODEL_DATA* modelData) {
	SetModelData(modelData);
	RenderS_SM();
};
void FBX_MODEL::RenderShadowModel(MODEL_DATA* modelData) {
	SetModelData(modelData);
	RenderS();
};
void FBX_MODEL::RenderModel(MODEL_DATA* modelData) {
	SetModelData(modelData);
	Render();
}
void FBX_MODEL::SetModelData(MODEL_DATA* modelData) {
	pos = modelData->pos;
	scale = modelData->scale;
	degree = modelData->degree;
	axis = modelData->axis;
	axisAngle = modelData->axisAngle;
	axisR = modelData->axisR;
	multiplyColor = modelData->multiplyColor;
}


MODEL_DATA::MODEL_DATA() {
	pos = Vector3(0.0f, 0.0f, 0.0f);
	degree = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	axis = Vector3(1.0f, 1.0f, 1.0f);
	axisAngle = 0;
	axisR = Vector3(0.0f, 0.0f, 0.0f);

	multiplyColor = DirectX::XMFLOAT4(1, 1, 1, 1);

}

void MODEL_DATA::DebugMode(float speed, bool quaternion,int strX, int strY) {
	KeyInput* KEY = KeyInput::GetInstance();

	char buf[256];
	WCHAR wbuf[256];
	sprintf_s(buf, sizeof(buf), "POS:X:%.2f:Y:%.2f:Z:%.2f", pos.X, pos.Y, pos.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 30, DXTK11LIB::StrColor{ 1,0,0,1 });
	sprintf_s(buf, sizeof(buf), "SCALE:X:%.2f:Y:%.2f:Z:%.2f", scale.X, scale.Y, scale.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 60, DXTK11LIB::StrColor{ 1,0,0,1 });
	sprintf_s(buf, sizeof(buf), "DEGREE:X:%.2f:Y:%.2f:Z:%.2f", degree.X, degree.Y, degree.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 90, DXTK11LIB::StrColor{ 1,0,0,1 });

	//Šgk
	if (KEY->key[DIK_LCONTROL] >= 1 && KEY->key[DIK_LSHIFT] >= 1) {
		if (KEY->key[DIK_W] >= 1) {
			scale.Y += speed;
		}
		if (KEY->key[DIK_S] >= 1) {
			scale.Y -= speed;
		}
		if (KEY->key[DIK_D] >= 1) {
			scale.X += speed;
		}
		if (KEY->key[DIK_A] >= 1) {
			scale.X -= speed;
		}
		if (KEY->key[DIK_X] >= 1) {
			scale.Z += speed;
		}
		if (KEY->key[DIK_Z] >= 1) {
			scale.Z -= speed;
		}
		if (KEY->key[DIK_UP] >= 1) {
			scale.X += speed;
			scale.Y += speed;
			scale.Z += speed;
		}
		if (KEY->key[DIK_DOWN] >= 1) {
			scale.X -= speed;
			scale.Y -= speed;
			scale.Z -= speed;
		}
		return;
	}
	//ˆÚ“®
	if (KEY->key[DIK_LSHIFT] >= 1) {
		if (KEY->key[DIK_W] >= 1) {
			pos.Y += speed;
		}
		if (KEY->key[DIK_S] >= 1) {
			pos.Y -= speed;
		}
		if (KEY->key[DIK_D] >= 1) {
			pos.X += speed;
		}
		if (KEY->key[DIK_A] >= 1) {
			pos.X -= speed;
		}
		if (KEY->key[DIK_X] >= 1) {
			pos.Z += speed;
		}
		if (KEY->key[DIK_Z] >= 1) {
			pos.Z -= speed;
		}
		return;
	}
	//‰ñ“]
	if (quaternion == false) {
		if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {

				degree.Y++;
			}
			if (KEY->key[DIK_A] >= 1) {
				degree.Y--;
			}

			if (KEY->key[DIK_W] >= 1) {
				degree.X++;
			}
			if (KEY->key[DIK_S] >= 1) {
				degree.X--;
			}

			if (KEY->key[DIK_X] >= 1) {
				degree.Z++;
			}
			if (KEY->key[DIK_Z] >= 1) {
				degree.Z--;
			}
			return;
		}
	}
	else {
		if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {

				axisAngle++;
			}
			if (KEY->key[DIK_A] >= 1) {
				axisAngle--;
			}
			return;
		}
	
	}
}


CAMERA::CAMERA() {
	pos = Vector3(0.0f, 0.0f, 0.0f);
	angle = Vector3(0.0f, 0.0f, 0.0f);

	Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
	Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z*-1, length, angle.X);

	focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1]*-1);
}
void CAMERA::Updata(bool local) {


	if(local){
		Pos2 returnPosXZ = GetCirclePoint(pos.X, pos.Z, length, angle.Y);
		Pos2 returnPosYZ = GetCirclePoint(pos.Y, pos.Z * -1, length, angle.X);
		focs = Vector3(returnPosXZ.p[0], returnPosYZ.p[0], returnPosXZ.p[1] * -1);

		Vector3 ang = angle;
		ang.Y = angle.Y * -1;
		DirectGraphics::GetInstance()->SetCameraLocal(pos, focs, Vector3(0, 1, 0), ang);
	}
	else {
		DirectGraphics::GetInstance()->SetCamera(pos, focs, Vector3(0, 1, 0));
	}

}

void CAMERA::Debug(float speed,int strX, int strY) {
	KeyInput* KEY = KeyInput::GetInstance();

	char buf[256];
	WCHAR wbuf[256];
	sprintf_s(buf, sizeof(buf), "POS:X:%.2f:Y:%.2f:Z:%.2f", pos.X, pos.Y, pos.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 30, DXTK11LIB::StrColor{ 1,0,0,1 });
	
	sprintf_s(buf, sizeof(buf), "DEGREE:X:%.2f:Y:%.2f:Z:%.2f", angle.X, angle.Y, angle.Z);
	mbstowcs_s(0, wbuf, 256, buf, _TRUNCATE);
	DXTK11LIB::DrawString(wbuf, strX + 0, strY + 90, DXTK11LIB::StrColor{ 1,0,0,1 });

	//ˆÚ“®
	if (KEY->key[DIK_LSHIFT] >= 1) {
		if (KEY->key[DIK_W] >= 1) {
			pos.Y += speed;
		}
		if (KEY->key[DIK_S] >= 1) {
			pos.Y -= speed;
		}
		if (KEY->key[DIK_D] >= 1) {
			pos.X += speed;
		}
		if (KEY->key[DIK_A] >= 1) {
			pos.X -= speed;
		}
		if (KEY->key[DIK_X] >= 1) {
			pos.Z += speed;
		}
		if (KEY->key[DIK_Z] >= 1) {
			pos.Z -= speed;
		}
		return;
	}
	//‰ñ“]
	if (KEY->key[DIK_LCONTROL] >= 1) {
			if (KEY->key[DIK_D] >= 1) {

				angle.Y++;
			}
			if (KEY->key[DIK_A] >= 1) {
				angle.Y--;
			}

			if (KEY->key[DIK_W] >= 1) {
				angle.X++;
			}
			if (KEY->key[DIK_S] >= 1) {
				angle.X--;
			}

			if (KEY->key[DIK_X] >= 1) {
				angle.Z++;
			}
			if (KEY->key[DIK_Z] >= 1) {
				angle.Z--;
			}
			return;
		}
	
}

