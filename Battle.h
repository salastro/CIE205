#pragma once

#include "Fighter.h" 
#include "Freezer.h" 
#include "Healer.h" 
#include "Enemies\Enemy.h"
#include "Castle\Castle.h"
#include "Generic_DS\Queue.h"
#include "Generic_DS/ArrayStack.h"
#include "Generic_DS/priQueue.h"
#include "GUI\GUI.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
using namespace std;

class Super_Sol;

// it is the controller of the project
class Battle
{
private:
	GUI* pGUI;
	Castle BCastle;
	int EnemyCount;	//the actual number of enemies in the game
	int ActiveCount = 0, FrostedCount = 0, KilledCount = 0;
	int ActiveFighters = 0, ActiveHealers = 0, ActiveFreezer = 0;
	int KilledFighters = 0, KilledHealers = 0, KilledFreezer = 0;
	int FrostedFighters = 0, FrostedHealers = 0, FrostedFreezer = 0;
	int CurrentTimeStep;
	int maxEnemiesToAttack = 0;
	int idcount;
	int idP;
	Result GameResult = ON_GOING;
	Queue<int> IDarray;
	//Enemy * BEnemiesForDraw[MaxEnemyCount]; // This Array of Pointers is used for drawing elements in the GUI
											// No matter what list type you are using to hold enemies, 
											// you must pass the enemies to the GUI function as an array of enemy pointers. 
											// At every time step, you should update those pointers 
											// to point to the current active enemies 
											// then pass the pointers list to the GUI function
											// 
	//List for all enemies in the battle
	Queue<Enemy*> Q_Inactive;		  //Queue of inactive enemies
	Queue<Enemy*> Q_Active;			 //Queue of Q_Active enemies to be passed for the healer
	priQueue<Fighter*> PQ_Fighters; // Priority Queue for fighters
	ArrayStack<Healer*> S_Healers; // Stack for healers
	Queue<Freezer*> Q_Freezers;	  // Queue for freezers
	Queue<Enemy*> Killed;		 //Queue of killed enemies
	Queue<Enemy*> Frosted;		//Queue of Frosted enemies
	Queue<double> Distances;  //Queue of distances of enemies
	
	Queue<Enemy*> SS_Super;
	Super_Sol* Super = nullptr;

	void UpdateActive();
	void UpdateFrost();

public:

	Battle();
	~Battle();
	void AddAllListsToDrawingList();  //Add enemy lists (inactive, active,.....) to drawing list to be displayed on user interface
	Castle* GetCastle();
	void RunSimulation();
	void Activation();		//check the inactive list and activate all enemies that has arrived
	void UpdateLists();
	void AttackandMove();
	
	void BGameResult();
	void setGameResult(Result result);
	Result getGameResult() const;
	void LoadFromFile(const std::string& filename); // Load enemies from a file
	void SaveToFile(const std::string& filename); // Save statiscs To a file
	
	//----------------- phase 2 ----------------------
	int Incrementtimestep();
	void PrintFromFile(const std::string& filename);

	void SaveFrames();
	void Simulator();
	//-------------------getters------------------
	bool IDunique();
	int getInactiveCount();
	int getActiveCount();
	int getFrostedCount();
	int getKilledCount();
	int getActiveFighters();
	int getActiveHealers();
	int getActiveFreezer();
	int getKilledFighters();
	int getKilledHealers();
	int getKilledFreezer();
	int getFrostedFighters();
	int getFrostedHealers();
	int getFrostedFreezer();
	Super_Sol* getSuper();
	void SetSuper(Super_Sol* s);
	//----------------- phase 2 ----------------------
	void Simulate();
	void InteractiveMode();
	void StepByStepMode();
	void SilentMode();
	
	//----------------- GUI ----------------------
	//void Counting();

	Battle* getBattle();
	std::string ResultToString();
};


