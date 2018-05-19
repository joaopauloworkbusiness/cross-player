#include "board.h"
#include "dictionary.h"
#include "cwplayer.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void cwplayer::menu() // The user enters an option
{
	unsigned int option;
	cout << "OPTIONS: " << endl;
	cout << "1 - Play Crosswords " << endl;
	cout << "0 - Exit" << endl << endl;

	cout << "Option ? ";
	cin >> option;

	switch (option)
	{

	case 1:
		playwords();
		break;

	case 0:
		cout << " End of program " << endl;
		exit(1);

	default:
		cout << "Option is invalid" << endl;
		cout << "Please enter a valid option" << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		menu();
	}
}

void cwplayer::playwords()
{
	cout << "Enter your name: ";
	
	while (true)
	{
		cin >> name;

		if (!cin.fail()) 
		break; 
		else {
			cin.clear();
			cin.ignore(10000, '\n');;
			cerr << "Invalid input";
			cout << "Enter your name ";
		}
	}

	ifstream boardfile;

	string boardfilename, dictionaryfilename, line;
	int lines = 1;
	int columns = 0;

	cout << "Enter the board file name you want play: ";
	cin >> boardfilename;

	//opening file
	boardfile.open(boardfilename);

	while (!boardfile.is_open())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "File " << "board" << " not found !\n";
		cout << "Enter the board file name you want play: ";
		cin >> boardfilename;
		boardfile.open(boardfilename);
	}
	getline(boardfile, dictionaryfilename);
	Dictionary dictionary1(dictionaryfilename);

	boardfile.ignore(10000,'\n') //skip the second line
}