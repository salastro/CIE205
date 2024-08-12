#pragma once
#include "..\Defs.h"
#include <ctime>
#include <cstdlib>
#include "..\Enemies\Enemy.h"
/*
#include "..\Fighter.h"
#include "..\Freezer.h"
#include "..\Healer.h"
*/
#include "..\Generic_DS/Queue.h"
#include "..\Generic_DS/ArrayStack.h"
#include "..\Generic_DS\priQueue.h"
#include <cmath>
#include <cstdlib> // For rand()
#include <iostream>
using namespace std;

class Enemy;
class Fighter;
class Healer;
class Freezer;
class Super_Sol;

class Castle
{
	double Health = 0;
	double MaxHealth = 0;
	double HealthThreshold = 0;
	CAST_STATUS status;
	double FreezingThreshold;
	double IceAccumulated = 0;
	double meltingRate = 0;
	int power = 0;
	int N = 0;
	ArrayStack<Super_Sol*> Super_soli;
	
	
	void FireBullet(Enemy* enemy);
	void ThrowIce(Enemy* enemy);

	int CalculatePriority(Enemy* fighter);
	bool ShouldThrowIce();
	void SuperSoldInit();

public:
	Castle(int power,double Health, int N);
	void SetHealth(double h);
	double GetHealth() const;
	double GetHealthThreshold() const;
	double GetMaxHealth() const;
	bool IsHealthThreshold();
	int GetN() const;
	int GetPower() const;
	double GetAccumulatedIce() const;
	void AddIce(double iceAmount);
	void MeltIce();

	void CalculateMeltingRate();

	double GetFreezingThreshold() const;

	CAST_STATUS GetStatus() const;
	void SetStatus(CAST_STATUS status);

	void Attack(priQueue<Fighter*>& fighters, ArrayStack<Healer*>& healers, Queue<Freezer*>& freezers);
	Super_Sol* ActivateSuper();
	bool ISS_Empty();

	string GetStatusString() const;
	string GetDetails() const;
};