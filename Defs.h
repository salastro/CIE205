#pragma once 


#define MaxDistance  60.0         // the maximum distance between any enemy and castle
#define MinDistance  2.0          // the minimum distance between any enemy and castle
#define MaxVerticalEnemies  15   // the maximum number of enemies that can be drawn in one quarter (region)
		                        // if they are in the same region and same distance
								// (will be drawn vertical to each other)
#define MaxEnemyCount 900       // the maximum number of enemies the application can work on



enum ENMY_TYPE	//types of enemies
{
	FIGHTER,	//fighter enemy
	HEALER,		//healer enemy
	FREEZER,	//freezer enemy
	SUPERSOLID,	//Super Solider enemy
	ENMY_TYPE_CNT	//Number of types

};


enum ENMY_STATUS	//enemy status
{
	INAC,	//inactive (not arrived yet, not involved in the battle yet)
	ACTV,	//Active (has arrived and involved in the battle)
	FRST,	//Frosted (cannot fight/act/move)
	KILD,	//Killed enemy
	ENMY_STATUS_CNT	//Status count
};


enum CAST_STATUS	//Castle status
{
	C_INAC,	//inactive (not involved in the battle yet)
	C_ACTV,	//Active (involved in the battle)
	C_FRST,	//Frosted (cannot fight/act/move)
	C_KILD,	//Killed castle
	CAST_STATUS_CNT	//Status count
};


enum PROG_MODE	//mode of the program interface
{
	MODE_INTR,	//interactive mode
	MODE_STEP,	//Step-by-step mode
	MODE_SLNT,	//Silent mode
	MODE_DEMO,	//Demo mode (for introductory phase only, should be removed in phases 1&2)
	MODE_CNT	//number of possible modes
};

enum Result
{
	ON_GOING,
	WIN,
	LOSE,
	DRAW,
	TOTAL
};

