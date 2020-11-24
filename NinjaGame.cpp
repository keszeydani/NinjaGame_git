// NinjaGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "Header/Map.h"
#include "Header/Engine.h"

//Lookup table for name generation
std::map<char, string> lookup_table{
	{'A', "KA"}, {'B', "ZU"}, {'C', "MI"},
    {'D', "TE"}, {'E', "KU"}, {'F', "LU"},
    {'G', "JI"}, {'H', "RI"}, {'I', "KI"},
    {'J', "ZU"}, {'K', "ME"}, {'L', "TA"},
    {'M', "RIN"}, {'N', "TO"}, {'O', "MO"},
    {'P', "NO"}, {'Q', "KE"}, {'R', "SHI"},
    {'S', "ARI"}, {'T', "CHI"}, {'U', "DO"},
    {'V', "RU"}, {'W', "MEI"}, {'X', "NA"},
    {'Y', "FU"}, {'Z', "ZI"},
    {'a', "KA"}, {'b', "ZU"}, {'c', "MI"},
    {'d', "TE"}, {'e', "KU"}, {'f', "LU"},
    {'g', "JI"}, {'h', "RI"}, {'i', "KI"},
    {'j', "ZU"}, {'k', "ME"}, {'l', "TA"},
    {'m', "RIN"}, {'n', "TO"}, {'o', "MO"},
    {'p', "NO"}, {'q', "KE"}, {'r', "SHI"},
    {'s', "ARI"}, {'t', "CHI"}, {'u', "DO"},
    {'v', "RU"}, {'w', "MEI"}, {'x', "NA"},
    {'y', "FU"}, {'z', "ZI"}
};


string generateName(string realName)
{
    string l_Result;
	
	if(realName.length()>=3)
	{
        for (std::string::size_type i = 0; i < 3; i++) {
            l_Result += lookup_table[(realName[i])];
        }
	}
    return l_Result;
};

int main(int argc, char* argv[])
{

    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::vector<std::string> all_args;
    string FirstName, LastName;
    char input;
	
    if (argc > 1) {

        all_args.assign(argv + 1, argv + argc);
    }

	if(all_args.size() >= 2)
	{
        FirstName = generateName(all_args[0]);
        LastName = generateName(all_args[1]);
	}
    
    Engine* gameEngine = Engine::getInstance();

    cout << "Dear " << FirstName << " " << LastName << "-san !\n";
    cout << "Let's get started!\n";
	if((all_args.size()) >= 3)
	{
        if(gameEngine->loadMapList(all_args[2]))
        {
            for (auto& t : gameEngine->getMaps())
            {
                gameEngine->processMap(t);
                //gameEngine->getCurrentMap().render();

                //Move or process the next step until Move() return true
                while (!(gameEngine->move()))
                {
                }
                gameEngine->getCurrentMap().reInitToEmpty();
                cout << "Do you want to process another map ? Y , N\n";
                cin >> input;
            	if(input != 'Y' && input != 'y')
            	{
                    cout << "Sayounara !\n";
                    break;
            	}
            }
        }
        else
        {
            cout << "Was not possible to load the map based on the URL\n";
        }
	}
    else
    {
        cout << "Input format should be: Firstname Lastname InputListAbsolutePath\n";
    }

    return 0;
}

