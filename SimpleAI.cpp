#include "SimpleAI.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <time.h>
#include <algorithm>
#include <vector>
#include "glew.h"
#include "const.h"
#include "struct.h"
SimpleAI::SimpleAI() {
	myMap = NULL;
	wayPointSet = false;
	pickUp = false;
	srand(time(NULL));
}
SimpleAI::SimpleAI(struct Map * map) {
	myMap = map;
	wayPointSet = false;
	pickUp = false;
}
float SimpleAI::huristic() {
	return 0;
}
void SimpleAI::pathFind(char AIID,bool*keyBuffer, Crate * Crates)
{
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;


	if ((*env->AIAmmo < (float)SHELLSTORAGE / 2 || *env->AIHP < (float)TANKHP * 11.0 / 16.0))
	{
		for (int i = 0; i < 9; i++)
		{
			if (*env->AIAmmo < (float)SHELLSTORAGE / 2 && Crates[i].isActive && Crates[i].type == 0)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
			if (*env->AIHP < (float)TANKHP * 11.0 / 16.0 && Crates[i].isActive && Crates[i].type == 2)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
		}
		pickUp = true;
		wayPointSet = true;
	}
	else {
		pickUp = false;
	}

	float minx = 100;
	float miny = 100;
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (myMap->isSolid[i][j] || myMap->color[i][j][0] == 7)
			{
				if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
				{
					minx = myMap->coord[i][j][0];
					miny = myMap->coord[i][j][1];
				}
			}
		}
	}

	float FX = (wayPoint[0] - (env->AIPos[0])) / (sqrt(((wayPoint[0] - (env->AIPos[0]))*(wayPoint[0] - (env->AIPos[0]))) + ((wayPoint[1] - (env->AIPos[1]))*(wayPoint[1] - (env->AIPos[1])))));
	float FY = (wayPoint[1] - (env->AIPos[1])) / (sqrt(((wayPoint[0] - (env->AIPos[0]))*(wayPoint[0] - (env->AIPos[0]))) + ((wayPoint[1] - (env->AIPos[1]))*(wayPoint[1] - (env->AIPos[1])))));
	// F = -k * x
	float FSX = 0.0;
	float FSY = 0.0;
	if (fabs(minx - (env->AIPos[0])) <= ((float)CUBESIZE * 1.5))
		FSX = SC * ((float)CUBESIZE * 1.5 - fabs(minx - (env->AIPos[0])));
	if (fabs(miny - (env->AIPos[1])) <= ((float)CUBESIZE))
		FSY = SC * ((float)CUBESIZE - fabs(miny - (env->AIPos[1])));
	if(FX >= 0)
		xdist = FX - FSX;
	else
		xdist = FX + FSX;
	if (FY >= 0)
		ydist = FY + FSY;
	else
		ydist = FY - FSY;
	/*BEGIN DEBUG*/
	if(false)
	{
		float height = 5;
		glBegin(GL_LINE_STRIP);
			glColor3f(std::min(FSX / 2, (float)1.0),
					  std::min(FSY / 2, (float)1.0),
					 0.0);
			glVertex3f(env->AIPos[0], height, env->AIPos[1]);
			glVertex3f(minx, height, miny);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glColor3f(0, 0, 1);
			glVertex3f(env->AIPos[0], height + 0.1, env->AIPos[1]);
			glVertex3f(wayPoint[0], height + 0.1, wayPoint[1]);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glColor3f(0, 1, 0);
			glVertex3f(env->AIPos[0], height + 0.2, env->AIPos[1]);
			glVertex3f(FSX, height + 0.2, FSY);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glColor3f(0, 1, 1);
			glVertex3f(env->AIPos[0], height + 0.3, env->AIPos[1]);
			glVertex3f(env->AIPos[0]+FX, height + 0.3, env->AIPos[1]+FY);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glColor3f(1, 0, 0);
			glVertex3f(env->AIPos[0], height + 0.4, env->AIPos[1]);
			glVertex3f(xdist, height + 0.4, ydist);
		glEnd();
	}
	/*END DEBUG*/
	targetDistance = (wayPoint[0] - (env->AIPos[0])) * (wayPoint[0] - (env->AIPos[0])) + (wayPoint[1] - (env->AIPos[1])) * (wayPoint[1] - (env->AIPos[1]));

	AIHullAngle = *env->AIHullAngle;
	angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	current = (float)((int)(AIHullAngle) % 360) * ((float)PI) / 180;
	if (angleSine >= 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine < 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine >= 0 && angleCosine < 0)
		angle = ((float)PI) - asin(angleSine);
	if (angleSine < 0 && angleCosine < 0)
		angle = ((float)PI) - asin(angleSine);

	if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180.0)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['d'] = true;
			keyBuffer['a'] = false;
		}
		else {
			keyBuffer['6'] = true;
			keyBuffer['4'] = false;
		}
	}
	if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180.0)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['a'] = true;
			keyBuffer['d'] = false;
		}
		else {
			keyBuffer['4'] = true;
			keyBuffer['6'] = false;
		}
	}
	if ((current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180.0) && (current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180.0) && (targetDistance > ((float)AIENGAGE)*((float)AIENGAGE) || pickUp))
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = true;
			keyBuffer['s'] = false;
		}
		else {
			keyBuffer['8'] = true;
			keyBuffer['5'] = false;
		}
	}
	else
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = false;
		}
		else {
			keyBuffer['8'] = false;
		}
		if (targetDistance <= ((float)AIENGAGE)*((float)AIENGAGE)/(5.0*5.0) && !pickUp)
		{
			wayPointSet = false;
		}
	}
}
void SimpleAI::pathFind(char AIID, bool*keyBuffer, std::vector<Crate> Crates)
{
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;


	if ((*env->AIAmmo < (float)SHELLSTORAGE / 2 || *env->AIHP < (float)TANKHP * 11.0 / 16.0))
	{
		for (std::vector<Crate>::iterator it = Crates.begin(); it != Crates.end(); ++it)
		{
			if (*env->AIAmmo < (float)SHELLSTORAGE / 2 && it->isActive && it->type == 0)
			{
				wayPoint[0] = it->X;
				wayPoint[1] = it->Y;
			}
			if (*env->AIHP < (float)TANKHP * 11.0 / 16.0 && it->isActive && it->type == 2)
			{
				wayPoint[0] = it->X;
				wayPoint[1] = it->Y;
			}
		}
		/*for (int i = 0; i < 9; i++)
		{
			if (*env->AIAmmo < (float)SHELLSTORAGE / 2 && Crates[i].isActive && Crates[i].type == 0)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
			if (*env->AIHP < (float)TANKHP * 11.0 / 16.0 && Crates[i].isActive && Crates[i].type == 2)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
		}*/

		pickUp = true;
		wayPointSet = true;
	}
	else {
		pickUp = false;
	}

	float minx = 100;
	float miny = 100;
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (myMap->isSolid[i][j] || myMap->color[i][j][0] == 7)
			{
				if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
				{
					minx = myMap->coord[i][j][0];
					miny = myMap->coord[i][j][1];
				}
			}
		}
	}

	float FX = (wayPoint[0] - (env->AIPos[0])) / (sqrt(((wayPoint[0] - (env->AIPos[0]))*(wayPoint[0] - (env->AIPos[0]))) + ((wayPoint[1] - (env->AIPos[1]))*(wayPoint[1] - (env->AIPos[1])))));
	float FY = (wayPoint[1] - (env->AIPos[1])) / (sqrt(((wayPoint[0] - (env->AIPos[0]))*(wayPoint[0] - (env->AIPos[0]))) + ((wayPoint[1] - (env->AIPos[1]))*(wayPoint[1] - (env->AIPos[1])))));
	// F = -k * x
	float FSX = 0.0;
	float FSY = 0.0;
	if (fabs(minx - (env->AIPos[0])) <= ((float)CUBESIZE * 1.5))
		FSX = SC * ((float)CUBESIZE * 1.5 - fabs(minx - (env->AIPos[0])));
	if (fabs(miny - (env->AIPos[1])) <= ((float)CUBESIZE))
		FSY = SC * ((float)CUBESIZE - fabs(miny - (env->AIPos[1])));
	if (FX >= 0)
		xdist = FX - FSX;
	else
		xdist = FX + FSX;
	if (FY >= 0)
		ydist = FY + FSY;
	else
		ydist = FY - FSY;
	/*BEGIN DEBUG*/
	if (false)
	{
		float height = 5;
		glBegin(GL_LINE_STRIP);
		glColor3f(std::min(FSX / 2, (float)1.0),
			std::min(FSY / 2, (float)1.0),
			0.0);
		glVertex3f(env->AIPos[0], height, env->AIPos[1]);
		glVertex3f(minx, height, miny);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 1);
		glVertex3f(env->AIPos[0], height + 0.1, env->AIPos[1]);
		glVertex3f(wayPoint[0], height + 0.1, wayPoint[1]);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 1, 0);
		glVertex3f(env->AIPos[0], height + 0.2, env->AIPos[1]);
		glVertex3f(FSX, height + 0.2, FSY);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 1, 1);
		glVertex3f(env->AIPos[0], height + 0.3, env->AIPos[1]);
		glVertex3f(env->AIPos[0] + FX, height + 0.3, env->AIPos[1] + FY);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glColor3f(1, 0, 0);
		glVertex3f(env->AIPos[0], height + 0.4, env->AIPos[1]);
		glVertex3f(xdist, height + 0.4, ydist);
		glEnd();
	}
	/*END DEBUG*/
	targetDistance = (wayPoint[0] - (env->AIPos[0])) * (wayPoint[0] - (env->AIPos[0])) + (wayPoint[1] - (env->AIPos[1])) * (wayPoint[1] - (env->AIPos[1]));

	AIHullAngle = *env->AIHullAngle;
	angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	current = (float)((int)(AIHullAngle) % 360) * ((float)PI) / 180;
	if (angleSine >= 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine < 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine >= 0 && angleCosine < 0)
		angle = ((float)PI) - asin(angleSine);
	if (angleSine < 0 && angleCosine < 0)
		angle = ((float)PI) - asin(angleSine);

	if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180.0)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['d'] = true;
			keyBuffer['a'] = false;
		}
		else {
			keyBuffer['6'] = true;
			keyBuffer['4'] = false;
		}
	}
	if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180.0)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['a'] = true;
			keyBuffer['d'] = false;
		}
		else {
			keyBuffer['4'] = true;
			keyBuffer['6'] = false;
		}
	}
	if ((current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180.0) && (current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180.0) && (targetDistance > ((float)AIENGAGE)*((float)AIENGAGE) || pickUp))
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = true;
			keyBuffer['s'] = false;
		}
		else {
			keyBuffer['8'] = true;
			keyBuffer['5'] = false;
		}
	}
	else
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = false;
		}
		else {
			keyBuffer['8'] = false;
		}
		if (targetDistance <= ((float)AIENGAGE)*((float)AIENGAGE) / (5.0*5.0) && !pickUp)
		{
			wayPointSet = false;
		}
	}
}
void SimpleAI::getMove(char AIID, bool*keyBuffer, Crate * Crates) {
	if (AIID == 'a' || AIID == 'A')
	{
		keyBuffer['e'] = false;
		keyBuffer['q'] = false;
		keyBuffer['w'] = false;
		keyBuffer['s'] = false;
		keyBuffer['a'] = false;
		keyBuffer['d'] = false;
		keyBuffer['f'] = false;
	}
	else {
		keyBuffer['9'] = false;
		keyBuffer['7'] = false;
		keyBuffer['8'] = false;
		keyBuffer['5'] = false;
		keyBuffer['4'] = false;
		keyBuffer['6'] = false;
		keyBuffer['0'] = false;
	}

	int state = IDLE;
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;

	// Assessment and State analysis
	if (*env->playerHP > 0)
	{
		// Assess
		xdist = env->playerPos[0] - (env->AIPos[0]);
		ydist = env->playerPos[1] - (env->AIPos[1]);
		targetDistance = sqrt(xdist * xdist + ydist * ydist);
		// Set State:

		if (// AI HP above 75% AND
			((float)*env->AIHP >= ((float)TANKHP * 3.0 / 4.0)) &&
			// AI Ammo above 25% AND
			((float)*env->AIAmmo >= ((float)SHELLSTORAGE / 4.0)) &&
			// Enemy Position further than 5 blocks
			(targetDistance > 5.0 * ((float)CUBESIZE))
			)
			state = ATTACK;

		else
			state = DEFEND;

		if (// Enemy is withing 3 block range AND
			targetDistance < 4.0 * ((float)CUBESIZE) && (
				// (Out of Ammo) OR 
			(*env->AIAmmo < 1) ||
				//(HP bellow 50%)
				(float)*env->AIHP < ((float)TANKHP * 2.0 / 4.0)
				)
			)
			state = RUN;

		if (0) // (ammo crate present AND Ammo less than 50%) 
			state = AQUIRE;

		if (0) // (ammo crate present AND Ammo more than or equal to 50%)
			state = ELIMINATECRATE;
	}

	// Act:
	switch (state)
	{
	case ATTACK:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < ((float)AIENGAGE));
		if (*env->AIAmmo < (float)SHELLSTORAGE / 2.0 || *env->AIHP < (float)TANKHP / 2.0)
		{
			float minx = 100;
			float miny = 100;
			for (int i = 0; i < 24; i++)
			{
				for (int j = 0; j < 14; j++)
				{
					if (myMap->isSolid[i][j] && myMap->color[i][j][0] < 5)
					{
						if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
						{
							minx = myMap->coord[i][j][0];
							miny = myMap->coord[i][j][1];
						}
					}
				}
			}
			if (sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)) < sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) && minx < 100)
			{
				targetPos[0] = minx;
				targetPos[1] = miny;
				isClear = true;
			}
			else
				isClear = false;
		}
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < AIENGAGE && *env->playerHP > 0);

		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (wayPoint[0] != env->playerPos[0])
			wayPointSet = false;
		if (!wayPointSet)
		{
			wayPoint[0] = env->playerPos[0];
			wayPoint[1] = env->playerPos[1];
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case DEFEND:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)((float)AIENGAGE)*2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((((float)AIENGAGE) * 2.0), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE) / 4.0);
			else
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE) / 4.0);
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (int)(((float)AIENGAGE) / 4.0);
			else
				wayPoint[1] = targetPos[1] - rand() % (int)(((float)AIENGAGE) / 4.0);
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case RUN:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)((float)AIENGAGE) * 2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180.0 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((((float)AIENGAGE)), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE / 2.0));
			else
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE / 2.0));
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (int)(((float)AIENGAGE / 2.0));
			else
				wayPoint[1] = targetPos[1] - rand() % (int)(((float)AIENGAGE / 2.0));
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case AQUIRE:

		break;
	case ELIMINATECRATE:

		break;
	}
}
void SimpleAI::getMove(char AIID, bool*keyBuffer, std::vector<Crate> Crates) {
	if (AIID == 'a' || AIID == 'A')
	{
		keyBuffer['e'] = false;
		keyBuffer['q'] = false;
		keyBuffer['w'] = false;
		keyBuffer['s'] = false;
		keyBuffer['a'] = false;
		keyBuffer['d'] = false;
		keyBuffer['f'] = false;
	}
	else {
		keyBuffer['9'] = false;
		keyBuffer['7'] = false;
		keyBuffer['8'] = false;
		keyBuffer['5'] = false;
		keyBuffer['4'] = false;
		keyBuffer['6'] = false;
		keyBuffer['0'] = false;
	}

	int state = IDLE;
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;

	// Assessment and State analysis
	if (*env->playerHP > 0)
	{
		// Assess
		xdist = env->playerPos[0] - (env->AIPos[0]);
		ydist = env->playerPos[1] - (env->AIPos[1]);
		targetDistance = sqrt(xdist * xdist + ydist * ydist);
		// Set State:

		if (// AI HP above 75% AND
			((float)*env->AIHP >= ((float)TANKHP * 3.0 / 4.0)) &&
			// AI Ammo above 25% AND
			((float)*env->AIAmmo >= ((float)SHELLSTORAGE / 4.0)) &&
			// Enemy Position further than 5 blocks
			(targetDistance > 5.0 * ((float)CUBESIZE))
			)
			state = ATTACK;

		else
			state = DEFEND;

		if (// Enemy is withing 3 block range AND
			targetDistance < 4.0 * ((float)CUBESIZE) && (
				// (Out of Ammo) OR 
			(*env->AIAmmo < 1) ||
				//(HP bellow 50%)
				(float)*env->AIHP < ((float)TANKHP * 2.0 / 4.0)
				)
			)
			state = RUN;

		if (0) // (ammo crate present AND Ammo less than 50%) 
			state = AQUIRE;

		if (0) // (ammo crate present AND Ammo more than or equal to 50%)
			state = ELIMINATECRATE;
	}

	// Act:
	switch (state)
	{
	case ATTACK:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < ((float)AIENGAGE));
		if (*env->AIAmmo < (float)SHELLSTORAGE / 2.0 || *env->AIHP < (float)TANKHP / 2.0)
		{
			float minx = 100;
			float miny = 100;
			for (int i = 0; i < 24; i++)
			{
				for (int j = 0; j < 14; j++)
				{
					if (myMap->isSolid[i][j] && myMap->color[i][j][0] < 5)
					{
						if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
						{
							minx = myMap->coord[i][j][0];
							miny = myMap->coord[i][j][1];
						}
					}
				}
			}
			if (sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)) < sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) && minx < 100)
			{
				targetPos[0] = minx;
				targetPos[1] = miny;
				isClear = true;
			}
			else
				isClear = false;
		}
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < AIENGAGE && *env->playerHP > 0);

		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (wayPoint[0] != env->playerPos[0])
			wayPointSet = false;
		if (!wayPointSet)
		{
			wayPoint[0] = env->playerPos[0];
			wayPoint[1] = env->playerPos[1];
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case DEFEND:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)((float)AIENGAGE)*2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((((float)AIENGAGE) * 2.0), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE) / 4.0);
			else
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE) / 4.0);
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (int)(((float)AIENGAGE) / 4.0);
			else
				wayPoint[1] = targetPos[1] - rand() % (int)(((float)AIENGAGE) / 4.0);
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case RUN:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)((float)AIENGAGE) * 2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * ((float)PI) / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = ((float)PI) - angle;
		if (current <= angle + ((float)AIMTHRESH) * ((float)PI) / 180 && current >= angle - ((float)AIMTHRESH) * ((float)PI) / 180.0 && isClear)
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = true;
			}
			else {
				keyBuffer['0'] = true;

			}
		else
			if (AIID == 'a' || AIID == 'A')
			{
				keyBuffer['f'] = false;
			}
			else {
				keyBuffer['0'] = false;

			}
			if (current > angle + ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['e'] = true;
					keyBuffer['q'] = false;
				}
				else {
					keyBuffer['9'] = true;
					keyBuffer['7'] = false;
				}
			}
			if (current < angle - ((float)AIMTHRESH) * ((float)PI) / 180)
			{
				if (AIID == 'a' || AIID == 'A')
				{
					keyBuffer['q'] = true;
					keyBuffer['e'] = false;
				}
				else {
					keyBuffer['7'] = true;
					keyBuffer['9'] = false;
				}
			}
	}
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((((float)AIENGAGE)), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE / 2.0));
			else
				wayPoint[0] = targetPos[0] - rand() % (int)(((float)AIENGAGE / 2.0));
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (int)(((float)AIENGAGE / 2.0));
			else
				wayPoint[1] = targetPos[1] - rand() % (int)(((float)AIENGAGE / 2.0));
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case AQUIRE:

		break;
	case ELIMINATECRATE:

		break;
	}
}