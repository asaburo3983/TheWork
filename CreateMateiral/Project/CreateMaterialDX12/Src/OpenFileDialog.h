#pragma once
#include "LowApplication.h"
#include <shobjidl.h> 
class OpenFileDialog {
private:
	
	TCHAR filePath[MAX_PATH];

public:
	void SetFilePath(LPCSTR _filter, LPCSTR _title);
	void SetFbxPath();
	void SetHLSLPath();
	void SetTexturePath();
	void SetFolderPath();
	char* GetFilePath() { return filePath; };
};