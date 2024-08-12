#include "Enemy.h"

Enemy::Enemy(int id, int arrTime, double maxHealth, double speed, double power)
	: ID(id), ArrvTime(arrTime), MaxHealth(maxHealth), speed(speed), Power(power)
{
	status = INAC;
	Curr_Health = maxHealth;
	ReloadCounter = 0;
	FreezingThreshold = MaxHealth / 2 + Power;
	meltingRate = 0;
}

Enemy::~Enemy(){ }

int Enemy::GetID() const
{
	return ID;
}

void Enemy::SetStatus(ENMY_STATUS s)
{
	status = s;
}

void Enemy::UpdateStatus()
{
	if (Curr_Health <= 0) {
		SetStatus(KILD);
	}
	else if (IceAccumulated >= FreezingThreshold || GetSpeed() == 0) {
		SetStatus(FRST);
		CalculateMeltingRate();
	}
	else {
		SetStatus(ACTV);
	}
}

ENMY_STATUS Enemy::GetStatus() const
{
	return status;
}


void Enemy::DecrementDist()
{
	// Calculate the new distance by subtracting the speed
	if (Distance >= MinDistance)
	Distance -= speed;

	// Ensure the distance does not fall below the minimum allowed distance
	if (Distance < MinDistance)
	{
		Distance = MinDistance;
	}
}

void Enemy::IncrementDist()
{
	// Calculate the new distance by subtracting the speed
	if (Distance < MaxDistance)
		Distance += speed;

	// Ensure the distance does not fall below the minimum allowed distance
	if (Distance >= MaxDistance)
	{
		Distance = MaxDistance;
	}
}

int Enemy::GetPower() const
{
	return Power;
}

void Enemy::SetDistance(int d)
{
	if (d >= MinDistance && d <= MaxDistance)
		Distance = d;
	else
		Distance = MaxDistance;
}

double Enemy::GetDistance() const
{
	return Distance;
}

double Enemy::GetxDistance()
{
	return Distance;
}

int Enemy::GetArrvTime() const
{
	return ArrvTime;
}

double Enemy::GetCurrHealth() const
{
	return Curr_Health;
}

void Enemy::SetCurrHealth(double H)
{
	if (H > 0)
	Curr_Health = H;
	else
	{
		Curr_Health = 0;
		SetStatus(KILD);
	}
	CalculateMeltingRate();
}

double Enemy::GetMaxHealth() const
{
	return MaxHealth;
}

double Enemy::GetSpeed() const
{
	return speed;
}

void Enemy::SetSpeed(double s)
{
	speed = speed;
}

void Enemy::SetType(ENMY_TYPE t)
{
	type = t;
}

ENMY_TYPE Enemy::GetType() const
{
	return type;
}

double Enemy::GetAccumulatedIce() const
{
	return IceAccumulated;
}

void Enemy::AddIce(double iceAmount)
{
	IceAccumulated += iceAmount;
	ApplyIceEffect();
	if (IceAccumulated >= FreezingThreshold)
	{
		SetStatus(FRST);
		MeltIce();
	}
}

void Enemy::MeltIce()
{
	CalculateMeltingRate();
	IceAccumulated -= meltingRate;
	if (IceAccumulated < FreezingThreshold)
	{
		UpdateStatus();
	}
}
double Enemy::GetFreezingThreshold() const
{
	return FreezingThreshold;
}

void Enemy::ApplyIceEffect()
{
	double  F_speed = GetSpeed() * (1 - IceAccumulated / FreezingThreshold);
	SetSpeed(F_speed);
}

void Enemy::CalculateMeltingRate()
{
	meltingRate = GetCurrHealth() / (2 * (GetMaxHealth() - Power));
	if (this->GetType() == SUPERSOLID)
	{
		meltingRate = 0;
	}
}

void Enemy::setKTS(int kts)
{
	this->KTS = kts;
}

int Enemy::getKTS() const
{
	return KTS;
}

void Enemy::setFST(int fst)
{
	this->FST = fst;
}

int Enemy::getFST() const
{
	return FST;
}

void Enemy::Move()
{
	//UpdateStatus();
	//A Frozen or Killed enemy stops
	if (GetStatus() == ACTV)
	{ 
		DecrementDist();
	}
	else {
		SetSpeed(0);
		return;
	}
	//An enemy with (health < + its original health) moves at its half original speed.
	if (Curr_Health < MaxHealth)
	{
		SetSpeed((speed / 2));
	}
}

void Enemy::Reload()
{
	ReloadCounter = ReloadPeriod;
}

bool Enemy::IsNear(void* target)
{
	return false;
}

//------------------------------------------------------
std::string Enemy::GetTypeString() const
{
	switch (type)
	{
	case FIGHTER:
		return "Fighter";
	case HEALER:
		return "Healer";
	case FREEZER:
		return "Freezer";
	case SUPERSOLID:
		return "Super Solider";
	default:
		return "Unknown";
	}
}

std::string Enemy::GetStatusString() const
{
	switch (status)
	{
	case INAC:
		return "Inactive";
	case ACTV:
		return "Active";
	case FRST:
		return "Frosted";
	case KILD:
		return "Killed";
	default:
		return "Unknown";
	}
}

std::string Enemy::GetDetails() const
{
	std::string details;
	details += "ID: " + std::to_string(ID) + "\n";
	details += "Arrival Time: " + std::to_string(ArrvTime) + "\n";
	details += "Distance: " + std::to_string(Distance) + "\n";
	details += "Max Health: " + std::to_string(MaxHealth) + "\n";
	details += "Current Health: " + std::to_string(Curr_Health) + "\n";
	details += "Speed: " + std::to_string(speed) + "\n";
	details += "Power: " + std::to_string(Power) + "\n";
	details += "Reload Period: " + std::to_string(ReloadPeriod) + "\n";
	details += "Reload Counter: " + std::to_string(ReloadCounter) + "\n";
	details += "Ice Accumulated: " + std::to_string(IceAccumulated) + "\n";
	details += "Freezing Threshold: " + std::to_string(FreezingThreshold) + "\n";
	details += "Melting Rate: " + std::to_string(meltingRate) + "\n";
	details += "Status: " + GetStatusString() + "\n";
	details += "Type: " + GetTypeString() + "\n";
	details += "First Shot time: " + std::to_string(getFST()) + "\n";
	details += "Killed time: " + std::to_string(getKTS()) + "\n";
	return details;
}