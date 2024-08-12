#include "Castle.h"

Castle::Castle(int power, double Health, int N)
{
    this->status = C_INAC;
    this->power = power;
    this->Health = Health;
    this->N = N;
    MaxHealth = Health;
    FreezingThreshold = MaxHealth / 5;
    HealthThreshold = MaxHealth / 4;
    srand(time(nullptr));
    CalculateMeltingRate();
    SuperSoldInit();
}

void Castle::SetHealth(double h)
{
	if(h > 0)
		Health = h;
    else
    {
        Health = 0; // killed
        SetStatus(C_KILD);
    }
    CalculateMeltingRate();
}

double Castle::GetHealth() const
{
	return Health;
}

double Castle::GetHealthThreshold() const
{
    return HealthThreshold;
}

double Castle::GetMaxHealth() const
{
    return MaxHealth;
}

bool Castle::IsHealthThreshold() 
{
    return Health <= HealthThreshold;
}

int Castle::GetN() const
{
    return N;
}

int Castle::GetPower() const
{
    return power;
}

double Castle::GetAccumulatedIce() const
{
    return IceAccumulated;
}

void Castle::AddIce(double iceAmount)
{
    IceAccumulated += iceAmount;
    if (IceAccumulated >= FreezingThreshold)
    {
        SetStatus(C_FRST);
        IceAccumulated = 0; // Reset accumulated ice after being frosted
    }
}

void Castle::MeltIce()
{
    if (GetStatus() != C_FRST) { return; }
    IceAccumulated -= meltingRate;
    if (IceAccumulated < FreezingThreshold)
    {
        if (Health > 0) 
        {
            SetStatus(C_ACTV);
        }
        else
        {
            SetStatus(C_KILD);
        }
    }
}

void Castle::CalculateMeltingRate()
{
    meltingRate = static_cast<int>(round(GetHealth() / (GetMaxHealth() - power)));
}


double Castle::GetFreezingThreshold() const
{
    return FreezingThreshold;
}

CAST_STATUS Castle::GetStatus() const
{
    return status;
}

void Castle::SetStatus(CAST_STATUS status)
{
    this->status = status;
}

int Castle::CalculatePriority(Enemy* fighter)
{
    return static_cast<int>(round(fighter->GetPower() + fighter->GetCurrHealth()) / fighter->GetDistance());
}


bool Castle::ShouldThrowIce()
{
    return (rand() % 100) < 20;
}


void Castle::FireBullet(Enemy* enemy)
{
    double damage = (1.0 / enemy->GetDistance()) * power;
    if (enemy->GetType() == HEALER) {
        damage /= 2;
    }
    enemy->SetCurrHealth(enemy->GetCurrHealth() - damage);
    if (enemy->GetCurrHealth() <= 0 && enemy->GetType() == HEALER && enemy->GetDistance() <= 5 ) {
        double health = GetHealth() * 1.03;
        if (health > MaxHealth) { health = MaxHealth; }
        SetHealth(health); 
    }
}

void Castle::ThrowIce(Enemy* enemy)
{
    double addice = abs(power - MaxHealth);
    enemy->AddIce(addice); 
}

void Castle::SuperSoldInit()
{
    for (int i = 0; i < 3; i++)
    {
        Super_Sol* Solider = new Super_Sol(i + 1, power, 5, 1, MaxHealth / 3, 3);
        Super_soli.push(Solider);
    }   
}

void Castle::Attack(priQueue<Fighter*>& fighters, ArrayStack<Healer*>& healers, Queue<Freezer*>& freezers)
{
    if (GetStatus() != C_ACTV) { return; }

    int enemiesAttacked = 0;
    // Attack fighters based on priority
    while (!fighters.isEmpty() && enemiesAttacked < N) {
        Fighter* fighter = nullptr;
        int Priority = 0;
        fighters.dequeue(fighter, Priority);
        if (ShouldThrowIce()) {
            ThrowIce(fighter);
        }
        else {
            FireBullet(fighter);
        }
        Priority = fighter->CalculatePriority();
        fighters.enqueue(fighter, Priority);

        enemiesAttacked++;
    }

    // Attack healers (Last-Come-First-Serve)
    while (!healers.isEmpty() && enemiesAttacked < N) {
        Healer* healer = nullptr;
        healers.pop(healer);
        if (ShouldThrowIce()) {
            ThrowIce(healer);
        }
        else {
            FireBullet(healer);
        }
        healers.push(healer);

        enemiesAttacked++;
    }

    // Attack freezers (First-Come-First-Serve)
    while (!freezers.isEmpty() && enemiesAttacked < N) {
        Freezer* freezer = nullptr;
        freezers.dequeue(freezer);
        if (ShouldThrowIce()) {
            ThrowIce(freezer);
        }
        else {
            FireBullet(freezer);
        }
        freezers.enqueue(freezer);

        enemiesAttacked++;
    }
}
//void Castle::Attack(priQueue<Fighter*>& fighters, ArrayStack<Healer*>& healers, Queue<Freezer*>& freezers)
//{
//    if (GetStatus() != C_ACTV) {
//        return;
//    }
//
//    int enemiesAttacked = 0;
//    // Attack fighters based on priority
//    while (!fighters.isEmpty() && enemiesAttacked < N) {
//        Fighter* fighter = nullptr;
//        int priority = 0;
//        fighters.peek(fighter, priority); // Peek to get the highest priority fighter
//        if (fighter == nullptr) break;
//
//        fighters.dequeue(fighter, priority); // Dequeue the highest priority fighter
//
//        if (ShouldThrowIce()) {
//            ThrowIce(fighter);
//        }
//        else {
//            FireBullet(fighter);
//        }
//        enemiesAttacked++;
//    }
//
//    // Attack healers (Last-Come-First-Serve)
//    while (!healers.isEmpty() && enemiesAttacked < N) {
//        Healer* healer = nullptr;
//        healers.pop(healer); // Pop the last added healer
//
//        if (ShouldThrowIce()) {
//            ThrowIce(healer);
//        }
//        else {
//            FireBullet(healer);
//        }
//        enemiesAttacked++;
//    }
//
//    // Attack freezers (First-Come-First-Serve)
//    while (!freezers.isEmpty() && enemiesAttacked < N) {
//        Freezer* freezer = nullptr;
//        freezers.dequeue(freezer); // Dequeue the first added freezer
//
//        if (ShouldThrowIce()) {
//            ThrowIce(freezer);
//        }
//        else {
//            FireBullet(freezer);
//        }
//        enemiesAttacked++;
//    }
//}

Super_Sol* Castle::ActivateSuper()
{
    if (Super_soli.isEmpty()) { return nullptr; }
    Super_Sol* Solider = nullptr;
    Super_soli.pop(Solider);
    return Solider;
}

bool Castle::ISS_Empty()
{
    return Super_soli.isEmpty();
}

//--------------------------------------
string Castle::GetStatusString() const
{
    switch (status)
    {
    case C_INAC:
        return "Inactive";
    case C_ACTV:
        return "Active";
    case C_FRST:
        return "Frosted";
    case C_KILD:
        return "Killed";
    default:
        return "Unknown";
    }
}

string Castle::GetDetails() const
{
    string details;
    details += "Health: " + std::to_string(Health) + "\n";
    details += "Max Health: " + std::to_string(MaxHealth) + "\n";
    details += "Health Threshold: " + std::to_string(HealthThreshold) + "\n";
    details += "Status: " + GetStatusString() + "\n";
    details += "Freezing Threshold: " + std::to_string(FreezingThreshold) + "\n";
    details += "Ice Accumulated: " + std::to_string(IceAccumulated) + "\n";
    details += "Power: " + std::to_string(power) + "\n";
    details += "N: " + std::to_string(N) + "\n";
    details += "Number of Super Soldiers: " + std::to_string(Super_soli.size()) + "\n"; // Assuming ArrayStack has a GetSize method
    return details;
}
