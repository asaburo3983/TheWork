#pragma once
#include "Object.h"
#include "MaterialLinker.h"

class EditList {
private:
	const Vector2 POS_RATIO = { 0,0 };
	const Vector2 SIZE_RATIO = { 0.2,1 };

	const Vector2 POS_RATIO_REMOVECHECK = { 0.3,0.3 };
	const Vector2 SIZE_RATIO_REMOVECHECK = { 0.5,0.5 };

	const Vector2 POS_RATIO_WARNING = { 0.3,0.3 };
	const Vector2 SIZE_RATIO_WARNING = { 0.5,0.5 };

	char inputNameCache[256][256];
	bool inputEnableCache[256];

	std::vector<MaterialLinker> materialLinker;

	bool removeCheckEnable = false;

	bool warning = false;
public:
	void Manager();
private:
	void UpdateMaterial();
	void AddMaterial();
	void RemoveMaterial();
	void RemoveCheck();
	void Warning();
};