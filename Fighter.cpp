//#include "Fighter.h"
#include "Enemies\Enemy.h"

Fighter::Fighter(int id, int arrTime, int power, int reloadPeriod, double Health, double Speed)
	: Enemy(id, arrTime, Health, Speed, power)
{
	type = FIGHTER;
	Power = power;
	ReloadPeriod = reloadPeriod;
}


int Fighter::CalculatePriority() const
{
	return static_cast<int>(round(this->GetPower() + this->GetCurrHealth()) / this->GetDistance());
}

void Fighter::Act(void* target)
{
	if (GetStatus() != ACTV) { return; }

	if (ReloadCounter > 0)
	{
		ReloadCounter--;
		return; // No actting while reload period
	}

	// Cast target to Castle pointer
	Castle* castle = static_cast<Castle*>(target);

	// Check if the cast was successful
	if (castle) {

		if (castle->GetStatus() == C_KILD) return; // castle is dead

		//Fighting
		double K = 1;

		// Damage Factor
		if (Curr_Health < (MaxHealth / 2)) { K = 0.5; }

		// Damage Magnitude
		double Damage = K * (Power / Distance);

		// Apply on castle
		castle->SetHealth(castle->GetHealth() - Damage);

		// After Attacking, start the reload process
		Reload();
		return;
	}

	Super_Sol* superSol = static_cast<Super_Sol*>(target);
	if (superSol) {

		if (superSol->GetStatus() != ACTV) return;

		//Fighting
		double K = 3;

		// Damage Factor
		if (Curr_Health < (MaxHealth / 2)) { K = 1.5; }

		// Damage Magnitude
		double Damage = K * (Power / Distance);

		// Apply on superSol
		superSol ->SetCurrHealth(superSol->GetCurrHealth() - Damage);

		// After Attacking, start the reload process
		Reload();
		return;
	}
}

bool Fighter::IsNear(void* target)
{
	Super_Sol* Super = static_cast<Super_Sol*>(target);
	double distanceToEnemy = abs(Super->GetDistance() - GetDistance());
	if (distanceToEnemy >= 4)
	{
		return true;
	}
	return false;
}
