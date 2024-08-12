//#include "Freezer.h"
#include "Enemies\Enemy.h"

Freezer::Freezer(int id, int arrTime, int power, int reloadPeriod, double Health, double Speed)
	: Enemy(id, arrTime, Health, Speed, power)
{
	type = FREEZER;
	Power = power;
	ReloadPeriod = reloadPeriod;
}

void Freezer::Act(void* target)
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
	if (!castle) return;
	
	double F = 2;

	// Freezing Factor
	if (Curr_Health < (MaxHealth / 2)) { F = 1.5; }

	if (castle->GetStatus() == C_FRST) return; // Castle is frosted

	// Freezing Magnitude
	double IceAmount = F * (Power / Distance);

	// Apply on castle
	castle->AddIce(IceAmount);

	// After Attacking, start the reload process
	Reload();
}
