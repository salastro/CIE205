//#include "Healer.h"
#include "Enemies\Enemy.h"

Healer::Healer(int id, int arrTime, int power, int reloadPeriod, double Health, double Speed)
	: Enemy(id, arrTime, Health, Speed, power), movingForward(true)
{
	
	type = HEALER;
	ReloadPeriod = reloadPeriod;
}

void Healer::Move()
{
	UpdateStatus();

	//A Frozen or Killed or inactive enemy stops
	if (GetStatus() != ACTV)
	{
		SetSpeed(0);
		return;
	}
	//An enemy with (health < + its original health) moves at its half original speed.
	if (Curr_Health < GetMaxHealth()) { SetSpeed((speed / 2)); }

	// Check the current movement direction
	if (movingForward)
	{
		// If moving forward and not at the minimum distance, decrease the distance
		if (Distance > MinDistance) { DecrementDist(); }
		
		// If at the minimum distance, switch to moving backward
		else { movingForward = false; }
	}
	else
	{
		// If moving backward and not at the maximum distance, increase the distance
		if (Distance < MaxDistance) { IncrementDist(); }

		// If at the maximum distance, switch to moving forward
		else { movingForward = true; }
	}
}

void Healer::Act(void* target)
{
	if (!GetStatus() == ACTV) { return; }

	if (ReloadCounter > 0)
	{
		ReloadCounter--;
		return; // No actting while reload period
	}

	//Check if the target is of type Queue<Enemy*>
	Queue<Enemy*>* ActiveEnemies = static_cast<Queue<Enemy*>*>(target);
	if (!ActiveEnemies) { return; }

	// piority queue to store enemies within healable distance
	priQueue<Enemy*> healableEnemies;
	Enemy* enemy = nullptr;
	
	// Iterate through the queue and find healable enemies
	for (int i = 0; i < ActiveEnemies->size(); i++)
	{
		ActiveEnemies->dequeue(enemy);
		int distanceToEnemy = abs(enemy->GetDistance() - GetDistance());

		if ((distanceToEnemy <= 2) && (enemy != this) && (enemy->GetStatus() != KILD) && (enemy->GetStatus() != FRST) && enemy->GetType() != SUPERSOLID)
		{
			healableEnemies.enqueue(enemy, distanceToEnemy);
		}
		ActiveEnemies->enqueue(enemy);
	}

	// Heal the enemies in the healable queue
	while (!healableEnemies.isEmpty())
	{
		int distanceToEnemy = 0;
		healableEnemies.dequeue(enemy, distanceToEnemy);
		distanceToEnemy = abs(enemy->GetDistance() - GetDistance());

		// Healing formula based on healer's health, enemy's current health, power,and distance
		double BASE_HEAL_AMOUNT = enemy->GetMaxHealth();
		double healAmount = BASE_HEAL_AMOUNT * (GetCurrHealth() / GetMaxHealth()) * (double(Power) / (distanceToEnemy + 1));
		double newHealth = enemy->GetCurrHealth() + healAmount;

		// Ensure the health does not exceed the maximum health
		if (newHealth > enemy->GetMaxHealth()) { newHealth = enemy->GetMaxHealth(); }

		// Apply the healing
		enemy->SetCurrHealth(newHealth);
	}

	// After healing, start the reload process
	Reload();
}