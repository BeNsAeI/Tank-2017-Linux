#pragma once
#ifndef MAP_H
#define MAP_H
#define PI 3.14159265
struct Map {
	float coord[24][14][3];
	float color[24][14][3];
	float angle[24][14];
	float HP[24][14];
	bool isSolid[24][14];
	bool MCM[24][14];
	bool isCrate[24][14];
};
#endif