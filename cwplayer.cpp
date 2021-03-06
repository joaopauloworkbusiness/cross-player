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
	
	dictionary1.show_tracks();
	//start_time();
	//addwordplayer();
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
		cin >> nameplayer;

		if (!cin.fail()) 
		break; 
		else {
			cin.clear();
			cin.ignore(10000, '\n');;
			cerr << "Invalid input";
			cout << "Enter your name ";
		}
	}


	ifstream boardfile, dictionaryfile;
	string dictionaryfilename;

	string boardfilename;

	cout << "Enter the board file name you want play: ";
	cin >> boardfilename;

	//opening file
	boardfile.open(boardfilename);

	while (!boardfile.is_open())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cerr << "File " << "board" << " not found !\n";
		cout << "Enter the board file name you want play: ";
		cin >> boardfilename;
		boardfile.open(boardfilename);
	}
	getline(boardfile, dictionaryfilename);
	Dictionary dictionary1(dictionaryfilename);
	this->dictionaryname = dictionaryfilename;
	boardfile.close();
	
	board1 = Board(boardfilename);
	//board1.fill_finished();
	
	map<string,string> mapall_words = board1.mapall_words();
	for (auto &it : mapall_words)
	{
		cout << it.first;

	}

	for (auto &it: mapall_words)
	{
		string position = it.first;
		string word = it.second;
		dictionary1.boardwordspair(position, word);

	}
	
	boardfile.close();
	board1.fill_finished();
	board1.create_playboard();
	board1.show();

}

void cwplayer::start_time()
{
	startime = clock();
}

void cwplayer::end_time()
{
	endtime = clock();
}

void cwplayer::addwordplayer()
{
	string position, word, option;
	bool continued = true;

	while (continued) {

		cout << "Position (LCD formart) ?: ";
		cin >> position;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cerr << "Invalid input. " << endl;
			exit(0);
		}

		cout << "Insert a valid word (- remove / ? - clue): ";
		cin >> word;
		if (word == "-")
		{
			board1.remove_word(position);
			insertedwords.erase(position);
		}
		else if (word == "?")
		{
			counterclues++;
			dictionary1.other_track(position);
		}
		else if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cerr << "Invalid word, nothing added." << endl;
			exit(1);
		}
		else
		{
			insertedwords.insert(pair<string, string>(position, word));
			board1.addword(position, word);
		}

		// check if board is complete
		if (insertedwords.size() == mapall_words.size())
		{
			cout << "The board is complete, do you want to finish? (yes/no): ";

			do
			{
				cin >> option;
				if (option == "no")
				{
					addwordplayer();
				}
				else if (option == "yes")
				{
					continued = false;
					if (finalchecking())
					{
						cout << "All the words that are on the board are valid." << endl;

					}
				}
				else {
					cin.clear();
					cin.ignore(1000, '\n');
					cerr << "Insert a valid option (yes/no): ";
				}
			} while (option != "yes" && option != "no");
		}
	}
}

bool cwplayer::finalchecking()
{
	bool valid = false;
	string word;
	
	for (const auto &it : insertedwords)
	{
		word = it.second;
		if (!dictionary1.validword(word)) {
			return valid;
		}
		else valid = true;
	}
	return valid;
}

bool cwplayer::finalcheck()
{
	for (const auto &it : mapall_words) {
		if (it.second != mapall_words.find(it.first)->second)
			return false;
	}
	return true;
}

void cwplayer::save_game()
{
	ofstream output;
	static unsigned int gamenumber = 0;
	gamenumber++;
	string filenameboard, text;

	stringstream boardname;
	boardname << 'b' << setw(3) << setfill('0') << gamenumber << "_p.text";
	filenameboard = boardname.str();
	
	string text1 = " ----- CROSSWORDS PLAYER ---- ";
    cout << "The file was saved in " << filenameboard << endl;
	output.open(filenameboard);	

	
	output << text1 << endl << endl;
	output << "Name: " << nameplayer << ";\n";
	output << "Number of times you asked for help: " << counterclues << ";\n";
	output << "Time taken to solve puzzle: " << time << " seconds.\n";;
	
	cout << "Your data was saved." << endl;
	
}

void cwplayer::finalgame()
{
	if (finalcheck())
	{
		end_time();

		// Results of the game
		double time;
		time = (double)(endtime - startime) / CLOCKS_PER_SEC;
		cout << endl;
		cout << "Very well, " << nameplayer << " just solved the crossword puzzles." << endl;
		cout << "You asked for " << counterclues << " tips" << endl;
		cout << "It took " << time << " seconds to complete the puzzle" << endl << endl;

		save_game();

		// ------------------ Again?
		cout << "Do you want to play the crossword again? (yes/no): " << endl;
		string option;

		while (true) {
			cin >> option;
			if (option == "yes") {
				menu();
				break;
			}
			else if (option != "no") {
				cerr << "Invalid option." << endl;
			}
		}
	}
}
