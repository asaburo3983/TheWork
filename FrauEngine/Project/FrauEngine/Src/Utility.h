#pragma once

#include "Library.h"

float GetOneFromColor(int _num);

class Vector3
{
public:
	float X = 0;
	float Y = 0;
	float Z = 0;

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
	void SetFloat3(float* _float3) {
		X = _float3[0];
		Y = _float3[1];
		Z = _float3[2];
	}
	void GetFloat3(float* _float3) {
		_float3[0] = X;
		_float3[1] = Y;
		_float3[2] = Z;
	}

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
	float R = 0;
	float G = 0;
	float B = 0;
	float A = 0;
	Color(float red, float green, float blue, float alpha)
	{
		if (red > 1) {
			red=GetOneFromColor(red);
		}
		if (green > 1) {
			green = GetOneFromColor(green);
		}
		if (blue > 1) {
			blue = GetOneFromColor(blue);
		}

		R = red;
		G = green;
		B = blue;
		A = alpha;
	}
	Color(float _color) {
		R = _color;
		G = _color;
		B = _color;
		A = 1;
	}
	Color()
	{
		R = G = B = A = 1.0f;
	}
};

class Time {
private:
	int time=0;
	int timeMax = 6000;
public:
	static Time* GetInstance() {
		static Time t;
		return &t;
	}
	void Update() {
		time = (time + 1) % timeMax;
	}
	int GetTime() { return time; }
	void SetTimeMax(int _max) { timeMax = _max; }
	int GetTimeMax() { return timeMax; }
};
//コンソール画面にフォーマット付き文字列を表示
void DebugOutputFormatString(const char* format, ...);

///ファイル名から拡張子を取得する
std::string GetExtension(const std::string& path);

///ファイル名から拡張子を取得する(ワイド文字版)
std::wstring GetExtension(const std::wstring& path);

///string(マルチバイト文字列)からwstring(ワイド文字列)を得る
std::wstring GetWideStringFromString(const std::string& str);

///wstring(ワイド文字列)からwstring(マルチバイト文字列)を得る
std::string GetStringFromWideString(std::wstring wstr);

void Split(char split_char, char* buffer, std::vector<std::string>& out);
std::string GetSplitEnd(std::string _str);

void Replace(char search_char, char replace_char, char* buffer);

WCHAR* GetWCharFromChar(const char* str);

DirectX::XMFLOAT3 GetXMFLOAT3FromVector3(Vector3 v3);
Vector3 GetVector3FromXMFLOAT3(DirectX::XMFLOAT3 f3);


