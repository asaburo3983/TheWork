#include "Utility.h"
using namespace DirectX;
using namespace std;

//コンソール画面にフォーマット付き文字列を表示
void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif
}
///ファイル名から拡張子を取得する
string GetExtension(const std::string& path) {
	auto idx = path.rfind('.');
	return path.substr(idx + 1, path.length() - idx - 1);
}
///ファイル名から拡張子を取得する(ワイド文字版)
wstring GetExtension(const std::wstring& path) {
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}

//String を　wStringに変換
std::wstring GetWideStringFromString(const std::string& str) {
	//呼び出し1回目(文字列数を得る)
	auto num1 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, nullptr, 0);

	std::wstring wstr;//stringのwchar_t版
	wstr.resize(num1);//得られた文字列数でリサイズ

	//呼び出し2回目(確保済みのwstrに変換文字列をコピー)
	auto num2 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);//一応チェック
	return wstr;
}
std::string GetStringFromWideString(std::wstring wstr){
	// wstring → SJIS
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// バッファの破棄
	delete[] cpMultiByte;

	// 変換結果を返す
	return(oRet);
}

//　「/」で文字を分解
void Split(char split_char, char* buffer, std::vector<std::string>& out)
{
	int count = 0;
	if (buffer == nullptr)
	{
		return;
	}

	int start_point = 0;

	while (buffer[count] != '\0')
	{
		if (buffer[count] == split_char)
		{
			if (start_point != count)
			{
				char split_str[256] = { 0 };
				strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
				out.emplace_back(split_str);
			}
			else
			{
				out.emplace_back("");
			}
			start_point = count + 1;
		}
		count++;
	}

	if (start_point != count)
	{
		char split_str[256] = { 0 };
		strncpy_s(split_str, 256, &buffer[start_point], count - start_point);
		out.emplace_back(split_str);
	}
}
// [/]での分割の末尾を得る
std::string GetSplitEnd(std::string _str) {
	// ファイル分解
	char buffer[256];

	sprintf_s(buffer, _str.c_str());
	
	// 記号統一
	Replace('\\', '/', buffer);
	std::vector<std::string> split_list;
	std::string replace_file_name = buffer;
	// 「/」で分解
	Split('/', buffer, split_list);
	return split_list[split_list.size() - 1];
}
//第一引数を第二引数に変換した文字列を返す　第三引数は変換する文字列
void Replace(char search_char, char replace_char, char* buffer)
{
	int len = (int)strlen(buffer);

	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == search_char)
		{
			buffer[i] = replace_char;
		}
	}
}

WCHAR* GetWCharFromChar(const char* str) {
	size_t tmpsize = strlen(str);//char型をWCHARに変換
	int  tmpsizei = tmpsize + 1;
	WCHAR* tmpwbuf = new WCHAR[tmpsizei];
	mbstowcs_s(0, tmpwbuf, tmpsizei, str, _TRUNCATE);

	return tmpwbuf;
}

DirectX::XMFLOAT3 GetXMFLOAT3FromVector3(Vector3 v3) {
	return DirectX::XMFLOAT3(v3.X, v3.Y, v3.Z);
}
Vector3 GetVector3FromXMFLOAT3(DirectX::XMFLOAT3 f3) {
	return Vector3(f3.x, f3.y, f3.z);
}


float GetOneFromColor(int _num) {
	return (float)_num / 256;
}