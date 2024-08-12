#pragma once

#include "..\Castle\Castle.h"
#include "..\Defs.h"
#include <string>
#include <iostream>
#include "..\CMUgraphicsLib\CMUgraphics.h"

class GUI;
// Enemy is the base class of each type of enemy
// Enemy should be an abstract class in next phases

class Enemy
{

protected:
	const int ID; //Each enemy has a unique ID (sequence number)
	const int ArrvTime;	//arrival time (time it starts to join battle)
	double Distance = MaxDistance;    //Horizontal distance between enemy & the tower of its region, Always positive (ranges from 2 to 60)
	double MaxHealth = 0;	//Enemy health
	double Curr_Health; // Current Health
	double speed = 0;
	int KTS = 0;
	int FST = 0;
	ENMY_STATUS status;	//status of the enemy (inactive, active, frosted, killed)
	ENMY_TYPE type; //status of the enemy (fighter, healer, freezer)

	int Power;
	int ReloadPeriod;
	int ReloadCounter; // Counter to manage reload periods

	double IceAccumulated = 0.0; // Amount of ice accumulated on the enemy
	double FreezingThreshold; // Ice freezing threshold
	double meltingRate; // Ice melting rate

public:
	Enemy(int id, int arrTime, double maxHealth = 0, double speed = 0, double power = 0); //would the distance be double??
	virtual ~Enemy();
	int Enemy::GetID() const;

	ENMY_STATUS GetStatus() const;
	void SetStatus(ENMY_STATUS s);
	void UpdateStatus();

	void DecrementDist();
	void IncrementDist();

	int GetPower() const;

	void SetDistance(int d);
	double GetDistance() const;
	double GetxDistance() ;


	int GetArrvTime() const;

	double GetCurrHealth() const;
	void SetCurrHealth(double H);
	double GetMaxHealth() const;

	double GetSpeed() const;
	void SetSpeed(double s);

	void SetType(ENMY_TYPE t);
	ENMY_TYPE GetType() const;

	double GetAccumulatedIce() const;

	double GetFreezingThreshold() const;
	void ApplyIceEffect();
	void CalculateMeltingRate();

	void setKTS(int kts);
	int getKTS() const;
	void setFST(int fts);
	int getFST() const;
	void MeltIce();
	// Virtual Functions: ------------------------------------------

	virtual void AddIce(double iceAmount);
	virtual void Move();	//All enemies can move
	virtual void Reload(); //Reloading
	virtual void Act(void* target) = 0;	//Acting means fighting or healing
	virtual bool IsNear(void* target);
	// Helper Debuging functions-----------------------------------------------------------

	std::string GetTypeString() const;

	std::string GetStatusString() const;

	std::string GetDetails() const;
};

class Super_Sol : public Enemy
{
private:
	int MaxDistofEnemies = MinDistance;
public:
	Super_Sol(int id = 0, int arrTime = 0, int power = 5, int reloadPeriod = 1, double Health = 50, double Speed = 3);
	void Act(void* target) override;
	void Moving();
	//void Move() override;
	void Actt(void* target);
	void AddIce(double iceAmount) override;
	void Reloading();
};

class Fighter : public Enemy
{
public:
	Fighter(int id, int arrTime, int power, int reloadPeriod = 0, double Health = 0, double Speed = 0);
	int CalculatePriority() const;
	void Act(void* target) override;
	bool IsNear(void* target) override;
};

class Freezer : public Enemy
{
public:
	Freezer(int id, int arrTime, int power, int reloadPeriod = 0, double Health = 0, double Speed = 0);
	void Act(void* target) override;
};

class Healer :public Enemy
{
private:
	bool movingForward;
public:
	Healer(int id, int arrTime, int power, int reloadPeriod = 0, double Health = 0, double Speed = 0);
	void Move() override;
	void Act(void* target) override;
};
