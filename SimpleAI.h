#pragma once
#ifndef SIMPLEAI_H
#define SIMPLEAI_H
#include <iostream>
#include <vector>
#include"const.h"
#include "Map.h"
#include "struct.h"
class SimpleAI {
public:
	SimpleAI();
	SimpleAI(struct Map * map);
	float huristic();
	void pathFind(char AIID,bool*keyBuffer, Crate * Crates);
	void pathFind(char AIID, bool*keyBuffer, std::vector<Crate> Crates);
	void getMove(char AIID,bool * keyBuffer, Crate * Crates);
	void getMove(char AIID, bool * keyBuffer, std::vector<Crate> Crates);
	struct InnerAIKB {
		bool isAI = false;
		char AIID = NULL;
		float * playerHP = NULL;
		float * playerPos = NULL;
		int * playerAmmo = NULL;
		float * playerHullAngle = NULL;
		float * playerTurretAngle = NULL;
		float * AIHP = NULL;
		float * AIPos = NULL;
		int * AIAmmo = NULL;
		float * AIHullAngle = NULL;
		float * AITurretAngle = NULL;
		SimpleAI * agent;
	};
	struct InnerAIKB * env;
private:
	float synaptic_weights;
	std::string plan[PLANSIZE];
	struct Map * myMap;
	float targetPos[2];
	float wayPoint[2];
	float tempWayPoint[2];
	bool wayPointSet;
	bool pickUp;
};

#endif