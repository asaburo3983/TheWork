#define _CRT_SECURE_NO_WARNINGS

#include "SaveLoadMap.h"
#include "stdio.h"

void LoadMap(MapTip* map , const char* filePath) {

	FILE* fp = fopen(filePath, "rb");

	fread(map, sizeof(MapTip), 1, fp);

	fclose(fp);

}

void SaveMap(MapTip map,const char* filePath) {

	FILE* fp = fopen(filePath, "wb");
	fwrite(&map, sizeof(MapTip), 1, fp);
	fclose(fp);

}