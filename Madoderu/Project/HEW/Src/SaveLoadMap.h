#pragma once
struct MapTip {
	int map[9][16];
};


void LoadMap(MapTip* map, const char* filePath);

void SaveMap(MapTip map, const char* filePath);