#pragma once
#ifndef STRUCT_H
#define STRUCT_H
#include "const.h"

struct Crate {
	int type;
	float X;
	float Y;
	int i;
	int j;
	bool isActive;
};
struct Shell {
	float x;
	float y;
	float angle;
	float startTime;
	int shooterId;
	bool active;
};
struct Bullet {
	float x;
	float y;
	float angle;
	float startTime;
	int shooterId;
	bool active;
};

enum Projections
{
	ORTHO,
	PERSP
};
enum ButtonVals
{
	RESET,
	QUIT
};
enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};


#endif
