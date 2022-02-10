#pragma once

#include "Library.h"


class Vector3
{
public:
	Vector3()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}
	Vector3 operator+(Vector3 tmp) {
		Vector3 v3;
		v3.X = X + tmp.X;
		v3.Y = Y + tmp.Y;
		v3.Z = Z + tmp.Z;
		return v3;
	}
	Vector3 operator*(float tmp) {
		Vector3 v3;
		v3.X = X * tmp;
		v3.Y = Y * tmp;
		v3.Z = Z * tmp;
		return v3;
	};
	Vector3 operator/(float tmp) {
		Vector3 v3;
		v3.X = X / tmp;
		v3.Y = Y / tmp;
		v3.Z = Z / tmp;
		return v3;
	};

	Vector3 operator*(Vector3 tmp) {
		Vector3 v3;
		v3.X = X * tmp.X;
		v3.Y = Y * tmp.Y;
		v3.Z = Z * tmp.Z;
		return v3;
	};

	float X = 0;
	float Y = 0;
	float Z = 0;
};
class Vector2
{
public:
	Vector2()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
	}
	Vector2(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
	float X = 0;
	float Y = 0;
};
class Color
{
public:
	float Red = 0;
	float Green = 0;
	float Blue = 0;
	float Alpha = 0;
	Color(float red, float green, float blue, float alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}
	Color()
	{
		Red = Green = Blue = Alpha = 1.0f;
	}
};


//�R���\�[����ʂɃt�H�[�}�b�g�t���������\��
void DebugOutputFormatString(const char* format, ...);

///�t�@�C��������g���q���擾����
std::string GetExtension(const std::string& path);

///�t�@�C��������g���q���擾����(���C�h������)
std::wstring GetExtension(const std::wstring& path);

///string(�}���`�o�C�g������)����wstring(���C�h������)�𓾂�
std::wstring GetWideStringFromString(const std::string& str);

///wstring(���C�h������)����wstring(�}���`�o�C�g������)�𓾂�
std::string GetStringFromWideString(std::wstring wstr);

void Split(char split_char, char* buffer, std::vector<std::string>& out);

void Replace(char search_char, char replace_char, char* buffer);

WCHAR* GetWCharFromChar(const char* str);

DirectX::XMFLOAT3 GetXMFLOAT3FromVector3(Vector3 v3);
Vector3 GetVector3FromXMFLOAT3(DirectX::XMFLOAT3 f3);

