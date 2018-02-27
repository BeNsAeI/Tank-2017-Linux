#pragma once
#ifndef CONST_H
#define CONST_H

const int GLUIFALSE = { false };

// Platfoarm compatibility
#ifndef WIN32
#include <unistd.h>
#define Sleep usleep
#endif

#define ESCAPE		0x1b
#define SPACEBAR	32
#define ENTER 13
#define CAMX -44;
#define CAMY 80;
#define START 0
#define END 1
#define TANKSCALE 0.25
#define OUTLINE 4.0f
#define MAPEDGEX 40
#define MAPEDGEY 70
#define BODY 4.75
#define CUBESIZE 6.0
#define SPAWN 45
#define TREESCALE 25
#define SHELLSCALE 0.045
#define SHELLSPEED 40000
#define ROCKTHRESH 25
#define REFLECT -1
#define SMOKECOUNT 3
#define TANKHP 3
#define BRICKHP 1
#define FRONTARMOR 0.75
#define SIDEARMOR 0.5
#define BACKARMOR 0.25
#define SHELLDAMAGE 1
#define SHELLBOUNCETHRESH 0.25
#define SHELLDURATION 0.03
#define SHELLMAX 50
#define SHELLSTORAGE 10
#define BOUNCETHRESH 0.1
#define CRATECAP 9
#define RELOADTIME 0.015
#define AMMOCRATE 0;
#define SMOKECRATE 1;
#define HPCRATE 2;
#define MINECRATE 3;
#define RELOADCRATE 3;
#define DAMAGECRATE 4;
#define RAMSPEEDCRATE 5;
#define ABRAMID 0
#define IS3ID 1
#define TOTAL_MS (180 * 1000)
#define NUMMODEL 1
#define BUFFER_OFFSET(i) ((void*)(i))
#define NUM_BUFFERS 13
#define NUM_SOURCES 13
#define _USE_MATH_DEFINES
#define PLANSIZE 3
#define AIMTHRESH 1
#define AIENGAGE 30
#define GRASSGRAINX 160
#define GRASSGRAINY 140
#define MENUMULTIPLIER 6
#define MENUXOFFSET 5
#define MENUZOFFSET 0
#define MENUYOFFSET -0.6
#define BUSHSCALE 2
#endif
