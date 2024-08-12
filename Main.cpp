#include "Battle.h"
#include "GUI\GUI.h"
#include <direct.h>   
int main()
{
	srand(time(nullptr));

	Battle* pGameBattle = new Battle;

	pGameBattle->LoadFromFile("Sample 4.txt");

	pGameBattle->RunSimulation();

    //The following hashed part is Running Python Scripts 
    const size_t size = 1024;
    char buffer[size];

    if (getcwd(buffer, size) != NULL) {
        string currentDir = buffer;
        string pythonInterpreter = "python";
        string script1 = currentDir + "/Analysis/Exceling.py";
        string script2 = currentDir + "/Analysis/Cleaning.py";
        string script3 = currentDir + "/Analysis/Visualizing.py";

        string scripts[] = { script1, script2, script3 };

        for (const string& script : scripts) {
            string command = pythonInterpreter + " \"" + script + "\"";
            cout << "Running script: " << script << " ..." << endl;

            int result = system(command.c_str());
            if (result == 0) {
                cout << "Script " << script << " executed successfully." << endl;
                cout << "----------------------------------------------------------------------" << '\n';
            }
            else {
                cerr << "Error executing script " << script << "." << endl;
            }
        }
    }
    else {
        cerr << "Error getting current working directory" << endl;
    }



   
    system("pause");
	return 0;
}