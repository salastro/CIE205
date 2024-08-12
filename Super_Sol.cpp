#include "Enemies/Enemy.h"


Super_Sol::Super_Sol(int id, int arrTime, int power, int reloadPeriod, double Health, double Speed)
    : Enemy(id, arrTime, Health, Speed, power)
{
    SetDistance(MinDistance);
    SetStatus(ACTV);
    type = SUPERSOLID;
    Power = power;
    ReloadPeriod = reloadPeriod;
    IceAccumulated = 0;
    meltingRate = 0;
}

void Super_Sol::Act(void* target) {
    if (GetStatus() != ACTV) {
        return;
    }

    if (ReloadCounter > 0) {
        ReloadCounter--;
        return; // No acting while reload period
    }

    Queue<Enemy*>* ActiveEnemies = static_cast<Queue<Enemy*>*>(target);
    if (!ActiveEnemies) {
        return;
    }

    priQueue<Enemy*> NearEnemies;
    Enemy* enemy = nullptr;

    // Iterate through the queue and find near enemies
    for (size_t i = 0; i < ActiveEnemies->size(); i++) 
    {
        ActiveEnemies->dequeue(enemy);
        int distanceToEnemy = abs(enemy->GetDistance() - GetDistance());

        if (distanceToEnemy <= 2 && this != enemy) {
            NearEnemies.enqueue(enemy, distanceToEnemy);
        }
        ActiveEnemies->enqueue(enemy);
    }

    // Kill up to 3 enemies in the NearEnemies queue
    for (int i = 0; i < 3 && !NearEnemies.isEmpty(); ++i) {
        int distanceToEnemy = 0;
        NearEnemies.dequeue(enemy, distanceToEnemy);
        enemy->SetCurrHealth(0);
    }

    // After killing, start the reload process
    Reload();
}

void Super_Sol::Moving()
{

    // A killed enemy stops
    if (GetStatus() != ACTV) {
        SetSpeed(0);
        return;
    }

    // Move towards the optimal fighting distance
    if (Distance > MaxDistofEnemies) {
        DecrementDist();
    }
    else if (Distance < MaxDistofEnemies) {
        IncrementDist();
    }
    else {
        SetSpeed(0); // Stop moving once the optimal position is reached
    }
}

/*void Super_Sol::Move() {
    //UpdateStatus();

    // A killed enemy stops
    if (GetStatus() != ACTV) {
        SetSpeed(0);
        return;
    }

    // Move towards the optimal fighting distance
    if (Distance > MaxDistofEnemies) {
        DecrementDist();
    }
    else if (Distance < MaxDistofEnemies) {
        IncrementDist();
    }
    else {
        SetSpeed(0); // Stop moving once the optimal position is reached
    }
}*/

void Super_Sol::Actt(void* target)
{
    if (GetStatus() != ACTV) {
        return;
    }

    if (ReloadCounter > 0) {
        ReloadCounter--;
        return; // No acting while reload period
    }

    Queue<Enemy*>* ActiveEnemies = static_cast<Queue<Enemy*>*>(target);
    if (!ActiveEnemies) {
        return;
    }

    priQueue<Enemy*> NearEnemies;
    Enemy* enemy = nullptr;

    // Iterate through the queue and find near enemies
    for (size_t i = 0; i < ActiveEnemies->size(); i++)
    {
        ActiveEnemies->dequeue(enemy);
        int distanceToEnemy = abs(enemy->GetDistance() - GetDistance());

        if (distanceToEnemy <= 2 && this != enemy) {
            NearEnemies.enqueue(enemy, distanceToEnemy);
        }
        ActiveEnemies->enqueue(enemy);
    }

    // Kill up to 3 enemies in the NearEnemies queue
    for (int i = 0; i < 3 && !NearEnemies.isEmpty(); ++i) {
        int distanceToEnemy = 0;
        NearEnemies.dequeue(enemy, distanceToEnemy);
        enemy->SetCurrHealth(0);
    }

    // After killing, start the reload process
    //Reload();
    Reloading();
}

void Super_Sol::AddIce(double iceAmount)
{
    iceAmount = 0;
    IceAccumulated = 0;
    meltingRate = 0;
}

void Super_Sol::Reloading()
{
    ReloadCounter = ReloadPeriod;
}
