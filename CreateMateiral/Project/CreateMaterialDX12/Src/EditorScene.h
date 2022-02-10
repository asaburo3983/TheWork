#pragma once
#include "Application.h"
#include "Object.h"


#include "MaterialLinker.h"
#include "BackObject.h"

class EditorScene : public Scene {
private:
	MaterialLinker materialLinker;

	BackObject backObject;
public:

	void LoadFrontLoad();
	void LoadInUpdata();
	void LoadInDraw();

	void Load();
	void Updata();
	void Draw();
	void UnLoad();
};
