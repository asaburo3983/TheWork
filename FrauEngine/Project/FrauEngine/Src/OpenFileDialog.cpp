#include "OpenFileDialog.h"



using namespace frauEngine;

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

	//���΃p�X�ւ̕ϊ�����
	//TODO ���΃p�X����ʂ̃t�H���_�ɃA�N�Z�X�ł��Ȃ� ../ ���g���Ȃ�
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

	LPCSTR filter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx;\0")
		TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("FBX�t�@�C����I�����܂��B");

	return SetFilePath(filter, title);
}
void OpenFileDialog::SetHLSLPath() {

	LPCSTR filter = TEXT("HLSL�t�@�C��(*.hlsl)\0*.hlsl;\0")
		TEXT("HLSL�t�@�C��(*.hlsl)\0*.hlsl\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("HLSL�t�@�C����I�����܂��B");

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

	//�J�����g�f�B���N�g���A�k��Ȃ�../���g���Ȃ��̂�
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

	LPCSTR filter = TEXT("PNG/TGA/JPG�t�@�C��(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("���ׂẲ摜�t�@�C��(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("PNG�t�@�C��(*.png)\0*.png\0")
		TEXT("TGA�t�@�C��(*.tga)\0*.tga\0")
		TEXT("JPG�t�@�C��(*.jpg)\0*.jpg\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("�摜�t�@�C����I�����܂��B");

	return SetFilePath(filter, title);
}