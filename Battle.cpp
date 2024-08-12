#include "Battle.h"

class GUI;
Battle::Battle() : BCastle(0, 0.0, 0)
{
	idP = 0;
	EnemyCount = 0;
	KilledCount = 0;
	ActiveCount = 0;
	FrostedCount = 0;
	CurrentTimeStep = 0;
	pGUI = NULL;
}

Battle::~Battle()
{
	delete pGUI;
	pGUI = nullptr;
	Killed.~Queue();
	Distances.~Queue();
	IDarray.~Queue();
	SS_Super.~Queue();
	delete Super;
	Super = nullptr;
}

int Battle::Incrementtimestep()
{
	return CurrentTimeStep++;
}

//Add ALL enemy lists (inactive, active,...etc) to drawing list to be displayed on user interface
void Battle::AddAllListsToDrawingList()
{
	//1- Add inactive queue to drawing list
	int InactiveCount = 0;
	Enemy* const* InactiveList = Q_Inactive.toArray(InactiveCount);
	for (int i = 0; i < InactiveCount; i++)
		pGUI->AddToDrawingList(InactiveList[i]);

	//2- Add active queue to drawing list
	int ActiveCount = 0;
	Enemy* const* ActiveList = Q_Active.toArray(ActiveCount);
	for (int i = 0; i < ActiveCount; i++)
		pGUI->AddToDrawingList(ActiveList[i]);
	
	int SSsize = 0;
	Enemy* const* SSList = SS_Super.toArray(SSsize);
	for (int i = 0; i < SSsize; i++)
		pGUI->AddToDrawingList(SSList[i]);

	int FrozenCount = 0;
	Enemy* const* FrozenList = Frosted.toArray(FrozenCount);
	for (int i = 0; i < FrozenCount; i++)
		pGUI->AddToDrawingList(FrozenList[i]);

	int KilledCount = 0;
	Enemy* const* KilledList = Killed.toArray(KilledCount);
	for (int i = 0; i < KilledCount; i++)
		pGUI->AddToDrawingList(KilledList[i]);
}

void Battle::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return;
	}
	// Read castle data
	double castleHealth;
	int castlePower;
	file >> castleHealth >> this->maxEnemiesToAttack >> castlePower;
	BCastle = Castle(castlePower, castleHealth, this->maxEnemiesToAttack);

	// Read number of enemies
	file >> EnemyCount;

	// Read enemy data + to do unique id 
	int id, type, arrivalTime, health, power, reloadPeriod, speed;
	for (int i = 0; i < EnemyCount; i++)
	{
		file >> id >> type >> arrivalTime >> health >> power >> reloadPeriod >> speed;
		idP = id;
		this->IDarray.enqueue(idP);

		switch (type)
		{
		case FIGHTER:
		{
			Fighter* pE = new Fighter(id, arrivalTime, power, reloadPeriod, double(health), double(speed));
			this->Q_Inactive.ASCT_enqueue(pE);
			break;
		}
		case HEALER:
		{
			Healer* pE = new Healer(id, arrivalTime, power, reloadPeriod, double(health), double(speed));
			this->Q_Inactive.ASCT_enqueue(pE);
			break;
		}
		case FREEZER:
		{
			Freezer* pE = new Freezer(id, arrivalTime, power, reloadPeriod, double(health), double(speed));
			this->Q_Inactive.ASCT_enqueue(pE);
			break;
		}
		default:
			std::cerr << "Error: Unknown enemy type " << type << std::endl;
			continue;
		}
	}
	if (!this->IDunique()) {
		file.close();
		system("Pause");
	}
	else {
		file.close();
	}
}

void Battle::SaveToFile(const std::string& filename)
{
	std::ofstream file(filename);

	if (!file)
	{
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return;
	}

	// Write the Game result
	file << "Game Result: " << ResultToString() << "\n" << endl;
	file << "-------------------------------------------------" << "\n" << endl;
	float AVFD = 0;
	float AVKD = 0;
	float AVLT = 0;

	Killed.bubbleSort();
	// Write killed Enemies data
	if(getGameResult() == WIN){
		Enemy* pE = nullptr;
		file << "KTS " << "ID \t" << "FD " << "KT " << "LT " << "\n" << endl;
		int size = Killed.size();
		for (int i = 0; i < size; i++)
		{
			Killed.dequeue(pE);
			int FD = abs(pE->getFST() - pE->GetArrvTime());
			AVFD += float(FD) / size;
			int KD = abs(pE->getKTS() - pE->getFST());
			AVKD += float(KD) / size;
			int LT = FD + KD;
			AVLT += float(LT) / size;
			file << pE->getKTS() << "  " << pE->GetID() << "\t" << FD << " " << KD << " " << LT << "\n" << endl;
		}
		file << "-----------------------------------------------" << "\n" << endl;
	}
	else if (getGameResult() == LOSE || getGameResult() == DRAW)
	{
		Enemy* pE = nullptr;
		int size = Killed.size();
		file << "KTS " << "ID \t" << "FD " << "KT " << "LT " << "\n" << endl;
		for (int i = 0; i < Killed.size(); i++)
		{
			Killed.dequeue(pE);
			int FD = pE->getFST() - pE->GetArrvTime();
			AVFD += float(FD) / size;
			int KD = pE->getKTS() - pE->getFST();
			AVKD += float(KD) / size;
			int LT = FD + KD;
			AVLT += float(LT) / size;
			file << pE->getKTS() << "  " << pE->GetID() << "\t" << FD << " " << KD << " " << LT << "\n" << endl;
		}
		file << "-----------------------------------------------" << "\n" << endl;
		
		if(!Q_Active.isEmpty()){
			int Asize = Q_Active.size();
			file << "ID " << "Type " << "Status " << "\n" << endl;
			for (int i = 0; i < Asize; i++)
			{
				Q_Active.dequeue(pE);
				file << pE->GetID() << " " << pE->GetTypeString() << " " << pE->GetStatusString() << "\n" << endl;
			}
		}
		if (!Frosted.isEmpty()) {
			int Fsize = Frosted.size();
			file << "ID " << "Type " << "Status " << "\n" << endl;
			for (int i = 0; i < Fsize; i++)
			{
				Frosted.dequeue(pE);
				file << pE->GetID() << " " << pE->GetTypeString() << " " << pE->GetStatusString() << "\n" << endl;
			}
		}
		file << "-------------------------------------------------" << "\n" << endl;
	}
	file << "Castle Total Damage: " << GetCastle()->GetMaxHealth() - GetCastle()->GetHealth() << "\n" << endl;
	file << "Total Enemies: " << EnemyCount << "\n" << endl;
	file << "Average First-Shot Delay: " << AVFD << "\n" << endl;
	file << "Average Kill Delay: " << AVKD << "\n" << endl;
	file << "Average Life Time: " << AVLT << "\n" << endl;
	file.close();
}

void Battle::PrintFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return;
	}

	// Read and print castle data
	double castleHealth;
	int castlePower;
	file >> castleHealth >> this->maxEnemiesToAttack >> castlePower;
	std::cout << "Castle Data:\n";
	std::cout << "Health: " << castleHealth << "\n";
	std::cout << "Max Enemies To Attack: " << this->maxEnemiesToAttack << "\n";
	std::cout << "Power: " << castlePower << "\n";

	// Read and print number of enemies
	file >> EnemyCount;
	std::cout << "Number of Enemies: " << EnemyCount << "\n";

	// Read and print enemy data
	int id, type, arrivalTime, health, power, reloadPeriod, speed;
	for (int i = 0; i < EnemyCount; i++)
	{
		file >> id >> type >> arrivalTime >> health >> power >> reloadPeriod >> speed;
		std::cout << "Enemy " << i + 1 << " Data:\n";
		std::cout << "ID: " << id << "\n";
		std::cout << "Arrival Time: " << arrivalTime << "\n";
		std::cout << "Health: " << health << "\n";
		std::cout << "Power: " << power << "\n";
		std::cout << "Reload Period: " << reloadPeriod << "\n";
		std::cout << "Speed: " << speed << "\n";

		switch (type)
		{
		case FIGHTER:
			std::cout << "Type: Fighter\n";
			break;
		case HEALER:
			std::cout << "Type: Healer\n";
			break;
		case FREEZER:
			std::cout << "Type: Freezer\n";
			break;
		default:
			std::cout << "Type: Unknown\n";
			break;
		}
	}

	file.close();
}

bool Battle::IDunique()
{
	const int* Arrp = IDarray.toArray(EnemyCount);
	const int* p2 = Arrp;

	for (int i = 0; i < EnemyCount; i++) {
		for (int j = i + 1; j < EnemyCount; j++) {
			if (Arrp[i] == p2[j]) {
				cout << "Error! The IDs are not unique!" << endl;
				return false;
			}
		}
	}
	return true;
}

//------------------------------------Logic part----------------------------------------------
//check the inactive list and activate all enemies that has arrived

void Battle::RunSimulation()
{
	pGUI = new GUI(getBattle());
	PROG_MODE mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		InteractiveMode();
		break;
	case MODE_STEP:
		StepByStepMode();
		break;
	case MODE_SLNT:
		SilentMode();
		break;
	case MODE_DEMO:
		Simulator();
		break;
	}
	delete pGUI;
}

void Battle::Activation()
{
	//castle activation
	this->GetCastle()->SetStatus(C_ACTV);

	//this->GetCastle()->SetHealth(GetCastle()->GetHealthThreshold());
	
	//super solider activation
	if (GetCastle()->IsHealthThreshold() && SS_Super.isEmpty())
	{
		SetSuper(GetCastle()->ActivateSuper());
		if (Super != nullptr) 
		{
			Super->SetStatus(ACTV);
			SS_Super.enqueue(Super);
		}
	}

	//Enemies activation
	Enemy* pE = nullptr;
	while (Q_Inactive.getFront())    // as long as there are more inactive enemies
	{
		if (Q_Inactive.getFront()->GetArrvTime() > CurrentTimeStep)    // no more arrivals at current time
			return;

		pE = Q_Inactive.getFront();

		if (!pE) return;

		switch (pE->GetType())
		{
		case FIGHTER:
		{
			Fighter* pFighter = dynamic_cast<Fighter*>(pE);
			if (pFighter)
			{
				Q_Inactive.dequeue(pE);    // remove enemy from the queue
				PQ_Fighters.enqueue(pFighter, pFighter->CalculatePriority());
				pFighter->SetStatus(ACTV);    // make status active
				Q_Active.enqueue(pFighter);    // enqueue to active queue
				ActiveFighters++;
			}
			pE->SetStatus(ACTV);  // Make status active
			break;
		}
		case HEALER:
		{
			Healer* pHealer = dynamic_cast<Healer*>(pE);
			if (pHealer)
			{
				Q_Inactive.dequeue(pE);    // remove enemy from the queue
				S_Healers.push(pHealer);
				pHealer->SetStatus(ACTV);    // make status active
				Q_Active.enqueue(pHealer);    // enqueue to active queue
				ActiveHealers++;
			}
			pE->SetStatus(ACTV);  // Make status active
			break;
		}
		case FREEZER:
		{
			Freezer* pFreezer = dynamic_cast<Freezer*>(pE);
			if (pFreezer)
			{
				Q_Inactive.dequeue(pE);    // remove enemy from the queue
				Q_Freezers.enqueue(pFreezer);
				pFreezer->SetStatus(ACTV);    // make status active
				Q_Active.enqueue(pFreezer);    // enqueue to active queue
				ActiveFreezer++;
			}
			pE->SetStatus(ACTV);  // Make status active
			break;
		}
		}
		ActiveCount++;
	}
}

void Battle::AttackandMove()
{
	this->GetCastle()->Attack(PQ_Fighters, S_Healers, Q_Freezers);
	int size = Q_Active.size(); 

	Enemy* tempEnemy = nullptr;

	for (int i = 0; i < size; i++) 
	{
		Q_Active.dequeue(tempEnemy);
		tempEnemy->Move(); 

		switch (tempEnemy->GetType()) {
		case FIGHTER:
			if (Super && tempEnemy->IsNear(Super))
			{
				tempEnemy->Act(Super);
			}
			tempEnemy->Act(this->GetCastle());
			break;
		case FREEZER:
			if (this->GetCastle()->GetStatus() == C_ACTV)
			{
				tempEnemy->Act(this->GetCastle());
			}
			break;
		case HEALER:
			tempEnemy->Act(&Q_Active);
			break;
		}
		Q_Active.enqueue(tempEnemy);
	}
	if (Super != nullptr && !SS_Super.isEmpty()) {
		Super->SetDistance(Distances.mostRepeatedItem());	
		Super->Moving();
		Super->Actt(&Q_Active);
	}

}

void Battle::UpdateLists()
{
	UpdateActive();
	UpdateFrost();
}

void Battle::UpdateActive()
{
	ActiveFighters = 0;
	ActiveHealers = 0;
	ActiveFreezer = 0;
	
	if(Super)
	{
		Distances.~Queue();
	}
	
	if (Q_Active.isEmpty()) { return; }

	int size = Q_Active.size();
	Enemy* tempEnemy = nullptr;
	for (int i = 0; i < size; i++)
	{
		Q_Active.dequeue(tempEnemy);
	
		if (Super && tempEnemy->GetStatus() == ACTV) {
			double dist = tempEnemy->GetxDistance();
			Distances.enqueue(dist);
		}
	
		if (tempEnemy->GetStatus() == FRST) 
		{
			//if (!Frosted.contains(tempEnemy)) {  }
			Frosted.enqueue(tempEnemy);
			if (tempEnemy->GetCurrHealth() < tempEnemy->GetMaxHealth() && tempEnemy->getFST() == 0) { tempEnemy->setFST(CurrentTimeStep); }
			switch (tempEnemy->GetType())
			{
			case FIGHTER:
				FrostedFighters++;
				break;
			case FREEZER:
				FrostedFreezer++;
				break;
			case HEALER:
				FrostedHealers++;
				break;
			}
		}
		else if (tempEnemy->GetStatus() == KILD)
		{
			//if (!Killed.contains(tempEnemy)){}
			Killed.enqueue(tempEnemy); 
			
			if (tempEnemy->getFST() == 0) { 
				tempEnemy->setFST(CurrentTimeStep);				
				tempEnemy->setKTS(CurrentTimeStep);
			}			
			else if (tempEnemy->getFST() != 0) { tempEnemy->setKTS(CurrentTimeStep); }

			switch (tempEnemy->GetType()) {
			case FIGHTER:
			{
				Fighter* pFighter = static_cast<Fighter*>(tempEnemy);
				int priority = pFighter->CalculatePriority();
				PQ_Fighters.removeItem(pFighter, priority);
				KilledFighters++;
				break;
			}
			case FREEZER:
				{
				Freezer* pFreezer = static_cast<Freezer*>(tempEnemy);
				Q_Freezers.removeItem(pFreezer);
				KilledFreezer++;
				break;
			}
			case HEALER:
			{
				Healer* pHealer = static_cast<Healer*>(tempEnemy);
				S_Healers.removeItem(pHealer);
				KilledHealers++;
				break;
			}
			}
		}
		else
		{
			if (tempEnemy->GetCurrHealth() < tempEnemy->GetMaxHealth() && tempEnemy->getFST() == 0) { tempEnemy->setFST(CurrentTimeStep); }
			Q_Active.enqueue(tempEnemy);
			// Count active enemies based on their type
			switch (tempEnemy->GetType())
			{
			case FIGHTER:
				ActiveFighters++;
				break;
			case FREEZER:
				ActiveFreezer++;
				break;
			case HEALER:
				ActiveHealers++;
				break;
			}
		}
	}
}

void Battle::UpdateFrost()
{
	if (Frosted.isEmpty()) { return; }
	int size = Frosted.size();
	Enemy* tempEnemy = nullptr;

	for (int i = 0; i < size; i++)
	{
		Frosted.dequeue(tempEnemy);
		if (tempEnemy->GetStatus() == ACTV) {
			//if (!Q_Active.contains(tempEnemy)) {  }
			Q_Active.enqueue(tempEnemy);
			switch (tempEnemy->GetType()) {
			case FIGHTER:
			{
				Fighter* pFighter = static_cast<Fighter*>(tempEnemy);
				PQ_Fighters.enqueue(pFighter, pFighter->CalculatePriority());
				ActiveFighters++;
				FrostedFighters--;
				break;
			}
			case FREEZER:
			{
				Freezer* pFreezer = static_cast<Freezer*>(tempEnemy);
				Q_Freezers.enqueue(pFreezer);
				ActiveFreezer++;
				FrostedFreezer--;
				break;
			}
			case HEALER:
			{
				Healer* pHealer = static_cast<Healer*>(tempEnemy);
				S_Healers.push(pHealer);
				ActiveHealers++;
				FrostedHealers--;
				break;
			}
			}
		}
		
		else if (tempEnemy->GetStatus() == KILD)
		{
			if (tempEnemy->getFST() == 0) {
				tempEnemy->setFST(CurrentTimeStep);
				tempEnemy->setKTS(CurrentTimeStep);
			}
			else if (tempEnemy->getFST() != 0) { tempEnemy->setKTS(CurrentTimeStep); }


			
			Killed.enqueue(tempEnemy);
			switch (tempEnemy->GetType()) {
			case FIGHTER:
			{
				Fighter* pFighter = static_cast<Fighter*>(tempEnemy);
				int priority = pFighter->CalculatePriority();
				PQ_Fighters.removeItem(pFighter, priority);
				KilledFighters++;
				FrostedFighters--;
				break;
			}
			case FREEZER:
			{
				Freezer* pFreezer = static_cast<Freezer*>(tempEnemy);
				Q_Freezers.removeItem(pFreezer);
				KilledFreezer++; 
				FrostedFreezer--;
				break;
			}
			case HEALER:
			{
				Healer* pHealer = static_cast<Healer*>(tempEnemy);
				S_Healers.removeItem(pHealer);
				KilledHealers++;
				FrostedHealers--;
				break;
			}
			}
		}
		else
		{
			Frosted.enqueue(tempEnemy);
		}
	}
	if (Super && Super->GetStatus() == KILD)
	{
		Super->setKTS(CurrentTimeStep);
		Killed.enqueue(Super);
	}
}

void Battle::SaveFrames()
{
	// Get the directory from the environment variable
	const char* envDir = std::getenv("GAME_DATA");
	std::string directory = (envDir != nullptr) ? envDir : "Game_data"; // Default to "data" if env variable is not set

	// Ensure the directory exists
	DWORD ftyp = GetFileAttributes(directory.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		// Directory does not exist, try to create it
		if (!CreateDirectory(directory.c_str(), NULL)) {
			if (GetLastError() != ERROR_ALREADY_EXISTS) {
				std::cerr << "Error: Cannot create directory " << directory << std::endl;
				return;
			}
		}
	}
	else if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY)) {
		// If it's not a directory, print an error message
		std::cerr << "Error: " << directory << " is not a directory" << std::endl;
		return;
	}

	// Construct the full filename
	std::string filename = directory + "\\" + std::to_string(CurrentTimeStep) + ".txt";
	std::ofstream file(filename);

	if (!file)
	{
		std::cerr << "Error: Cannot open file " << filename << std::endl;
		return;
	}

	// Write the current time step
	file << "Current Time Step: " << CurrentTimeStep << std::endl;

	// Write castle details
	file << "Castle data: " << std::endl;
	file << this->GetCastle()->GetDetails();
	file << "-------------------------------- " << std::endl;
	
	// Write Super Solider details
	if (Super)
	{
		file << "Super Solider: " << std::endl;
		file << Super->GetDetails();
		file << "-------------------------------- " << std::endl;
	}

	// Write active enemies data
	file << "Active Enemies data:" << std::endl;
	Q_Active.printQueuex(file);
	file << "-------------------------------- " << std::endl;


	// Write frozen enemies data
	file << "Frozen Enemies data:" << std::endl;
	Frosted.printQueuex(file);
	file << "-------------------------------- " << std::endl;

	// Write killed enemies data
	file << "Killed Enemies data:" << std::endl;
	Killed.printQueuex(file);
	file << "-------------------------------- " << std::endl;

	// Write game result
	file << "Game Result: " << ResultToString() << std::endl;
	file << "-------------------------------- " << std::endl;

	file.close();
}

void Battle::Simulator()
{
	// Run the simulation
	while (this->getKilledCount() != EnemyCount && this->GetCastle()->GetHealth() != 0) 
	{
		Incrementtimestep();
		Activation();
		AttackandMove();
		UpdateLists();
		
		SaveFrames();		
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		pGUI->UpdateInterface(CurrentTimeStep);
		Sleep(10);
	}

	// Last update
	pGUI->ResetDrawingList();
	AddAllListsToDrawingList();
	pGUI->UpdateInterface(CurrentTimeStep);
	
	pGUI->ClearStatusBar();

	pGUI->ResultStatusBar();

	SaveToFile("Statistics.txt");
	pGUI->waitForClick();
}

//--------------------------getters-----------------------------
Battle* Battle::getBattle()
{
	return this;
}


Castle* Battle::GetCastle()
{
	return &BCastle;
}

int Battle::getInactiveCount()
{
	return Q_Inactive.size();
}

int Battle::getActiveCount() {
	return Q_Active.size();
}

int Battle::getFrostedCount() {
	return Frosted.size();
}

int Battle::getKilledCount() {
	return Killed.size();
}

int Battle::getActiveFighters() {
	return PQ_Fighters.size();
}

int Battle::getActiveHealers() {
	return S_Healers.size();
}

int Battle::getActiveFreezer() {
	return Q_Freezers.size();
}

int Battle::getKilledFighters() {
	return KilledFighters;
}

int Battle::getKilledHealers() {
	return KilledHealers;
}

int Battle::getKilledFreezer() {
	return KilledFreezer;
}

int Battle::getFrostedFighters() {
	return FrostedFighters;
}

int Battle::getFrostedHealers() {
	return FrostedHealers;
}

int Battle::getFrostedFreezer() {
	return FrostedFreezer;
}

Super_Sol* Battle::getSuper()
{
	return Super;
}

void Battle::SetSuper(Super_Sol* s)
{
	if (Super != nullptr)
	{
		delete Super; // Deallocate existing Super object to prevent memory leaks
	}
	Super = s;
}


void Battle::Simulate()
{
	Incrementtimestep();
	Activation();
	AttackandMove();
	UpdateLists();
}

//-----------------------Game Modes-------------------------------
void Battle::InteractiveMode()
{
	pGUI->UpdateInterface(CurrentTimeStep);
	pGUI->PrintMessage("Interactive Mode: Click to take the next step.");
	pGUI->waitForClick();
	while (this->getKilledCount() != EnemyCount && this->GetCastle()->GetHealth() != 0)
	{

		pGUI->UpdateInterface(CurrentTimeStep);
		pGUI->waitForClick();
		Simulate();
	}
	pGUI->UpdateInterface(CurrentTimeStep);
	pGUI->ClearStatusBar();
	pGUI->ResultStatusBar();
	SaveToFile("Statistics.txt");
	pGUI->waitForClick();
}

void Battle::StepByStepMode()
{
	pGUI->UpdateInterface(CurrentTimeStep);
	pGUI->PrintMessage("Step by Step Mode: Wait for one second.");
	Sleep(1000);
	while (this->getKilledCount() != EnemyCount && this->GetCastle()->GetHealth() != 0)
	{
		pGUI->UpdateInterface(CurrentTimeStep);
		Sleep(1000);
		Simulate();
	}

	pGUI->UpdateInterface(CurrentTimeStep);
	pGUI->ClearStatusBar();
	pGUI->ResultStatusBar();
	SaveToFile("Statistics.txt");
	pGUI->waitForClick();
}

void Battle::SilentMode()
{
	while (this->getKilledCount() != EnemyCount && this->GetCastle()->GetHealth() != 0)
	{
		Simulate();
	}
	SaveToFile("Statistics.txt");
}

void Battle::BGameResult()
{
	if (this->getKilledCount() == EnemyCount)
	{
		setGameResult(WIN);
	}
	else if (this->GetCastle()->GetHealth() == 0)
	{
		setGameResult(LOSE);
	}
	else if (this->GetCastle()->GetHealth() == 0 && KilledCount == EnemyCount)
	{
		setGameResult(DRAW);
	}
	else
	{
		setGameResult(ON_GOING);
	}
}

void Battle::setGameResult(Result result)
{
	GameResult = result;
}

Result Battle::getGameResult() const
{
	return GameResult;
}
std::string Battle::ResultToString()
{
	BGameResult();
    switch (getGameResult())
    {
        case WIN:
            return "WIN All Enemies are Killed!!";
        case LOSE:
            return "Lose The Caslte is Dead!!";
        case DRAW:
            return "DRAW";
        case ON_GOING:
            return "ON_GOING";
        default:
            return "UNKNOWN";
    }
}
