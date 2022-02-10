#include "OpenFileDialog.h"



using namespace lowApp;

void OpenFileDialog::SetFilePath(LPCSTR _filter, LPCSTR _title) {


	auto lowApp = LowApplication::GetInstance();
	static TCHAR currentPath[MAX_PATH];
	EnableWindow(*lowApp->GetHWND(), false);

	if (currentPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, currentPath);
	}
	OPENFILENAME openFileName;
	ZeroMemory(&openFileName, sizeof(openFileName));
	ZeroMemory(&filePath, sizeof(filePath));
	
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = *lowApp->GetHWND();
	openFileName.lpstrInitialDir = currentPath;
	openFileName.lpstrFile = filePath;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = _filter;
	openFileName.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	GetOpenFileName(&openFileName);

	//相対パスへの変換処理
	//TODO 相対パスより上位のフォルダにアクセスできない ../ が使えない
	CString str = filePath;
	str.Replace(currentPath, "");
	while (1) {

		int tmp = str.Find("\\");

		if (tmp == std::string::npos)break;

		str.Replace("\\", "/");
	}
	str.Delete(0);
	_stprintf_s(filePath, str);

	EnableWindow(*lowApp->GetHWND(), true);
}
void OpenFileDialog::SetFbxPath() {

	LPCSTR filter = TEXT("FBXファイル(*.fbx)\0*.fbx;\0")
		TEXT("FBXファイル(*.fbx)\0*.fbx\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("FBXファイルを選択します。");

	return SetFilePath(filter, title);
}
void OpenFileDialog::SetHLSLPath() {

	LPCSTR filter = TEXT("HLSLファイル(*.hlsl)\0*.hlsl;\0")
		TEXT("HLSLファイル(*.hlsl)\0*.hlsl\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("HLSLファイルを選択します。");

	return SetFilePath(filter, title);
}

void OpenFileDialog::SetFolderPath() {

	IFileOpenDialog* fileOpen;
	auto lowApp = LowApplication::GetInstance();
	static TCHAR currentPath[MAX_PATH];
	EnableWindow(*lowApp->GetHWND(), false);

	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpen));

	DWORD option = 0;
	fileOpen->GetOptions(&option);
	fileOpen->SetOptions(option | FOS_PICKFOLDERS);

	fileOpen->Show(NULL);

	IShellItem* item;
	fileOpen->GetResult(&item);
	PWSTR pszFilePath;
	item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);

	std::string path = GetStringFromWideString(pszFilePath);
	sprintf_s(filePath, path.c_str());
	CString str = filePath;

	//カレントディレクトリ、遡れない../が使えないので
	if (currentPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, currentPath);
	}
	str.Replace(currentPath, "");
	while (1) {

		int tmp = str.Find("\\");

		if (tmp == std::string::npos)break;

		str.Replace("\\", "/");
	}
	str.Delete(0);
	str += "/";
	_stprintf_s(filePath, str);
	

	CoTaskMemFree(pszFilePath);
	
	item->Release();
	fileOpen->Release();
	EnableWindow(*lowApp->GetHWND(), true);
}

void OpenFileDialog::SetTexturePath() {

	LPCSTR filter = TEXT("PNG/TGA/JPGファイル(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("すべての画像ファイル(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("PNGファイル(*.png)\0*.png\0")
		TEXT("TGAファイル(*.tga)\0*.tga\0")
		TEXT("JPGファイル(*.jpg)\0*.jpg\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("画像ファイルを選択します。");

	return SetFilePath(filter, title);
}