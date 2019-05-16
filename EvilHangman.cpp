
#include <string>
#include <iostream>
#include<unordered_map>
#include "RBTreeMap.h"
// Any other classes you make

using namespace std;

bool debug;

bool PromptYesNoMessage(string msg) {
	while (true) {
		cout << msg << "(y/n) ";
		string response;
		cin >> response;
		if (response == "y")
			return true;
		if (response == "n")
			return false;
		cout << "Please enter y or n..." << endl;
	}
}

int PromptGuesses() 
{
	while (true) {
		int len;
		cout << "Please enter how many guesses: ";
		cin >> len;
		if (len > 0)
			return len;
	}
}

int PromptWordLength()
{
	int len;

	// Your code here...

	return len;
}

string PromptGuess(string& guessedLetters)
{
	string guess = "";

	// Your code here...

	return guess;
}

bool isWin(string guessedProgress)
{
	return guessedProgress.find("*") == string::npos;
}

void PlayEvilHangman(string file) 
{
	// Your code here...
}

int main()
{
	while (PromptYesNoMessage("So you still want to play this game"))
	{

		string file; //"dictionary.txt";
		int length;
		int guess;
		string answer;
		cout << "Enter dictionary file: ";
		cin >> file;
		cout << "Please enter the word length:";
		cin >> length;
		cout << "Please enter how many guesses:";
		cin >> guess;
		//to check if file name is correct or not
		ifstream dict(file);
		if (!dict.good()) {
			cout << "Could not open file " << file << endl;
			exit(0);
		}
		string line;
		string ojie_pattern;
		//pattern with stars initialization
		for (int i = 1; i <= length; i++)
		{
			ojie_pattern.append("*");
			answer.append("*");
		}
		//initially storing all the words with same length in a map

		unordered_map<string, vector<string>> map;
		string pattern = ojie_pattern;
		while (getline(dict, line)) {
			if (line.length() == length)
			{
				//ut << line << endl;
				if (map.find(pattern) == map.end())
				{
					vector<string> temp;
					temp.push_back(line);
					map[pattern] = temp;
				}
				else
				{
					vector<string>temp(map.at(pattern));
					temp.push_back(line);
					map[pattern] = temp;
				}
			}
		}
		cout << "Guesses left:" << guess << endl;
		vector<string> x(map.at(ojie_pattern));
		//initializing tree for further storage of words
		RBTreeMap *tree = new RBTreeMap;
		tree->add(pattern, x);
		string guess_s;
		int count = 0;
		Node  *w = nullptr;
		//string c;
		//game is on untill we finish the guess or we get the final word
		while ((count != length) && (guess >= 1))
		{
			string c;
			c.clear();
			w = nullptr;
			string pattern = answer;
			cout << "Enter your guess:" << endl;
			cin >> c;
			if (guess_s.find(c) != std::string::npos)
			{
				cout << "Already entered, please insert another one" << endl;
				continue;
			}
			guess_s.append(c);
			w = tree->getMax();
			cout << "Guessed words: " << guess_s << endl;
			//cout << w->key << w->length << endl;
			vector<string> x(tree->getInfo(w->key, w->length));
			//cout << x.size() << endl;
			map.clear();
			//iterate over the pattern with maximum occurence 
			for (int i = 0; i < x.size(); i++)
			{
				//cout << " q" << endl;
				pattern = answer;
				string temps = x.at(i);
				//check if the char is in string ,if not the word will be stored in a star pattern else appropriate pattern
				if (temps.find(c[0]) == std::string::npos)
				{
					//cout << temps << endl;
				}
				else
				{
					for (int j = 0; j < temps.length(); j++)
					{
						if (temps[j] == c[0])
						{
							pattern[j] = c[0];
						}
					}
				}

				if (map.find(pattern) == map.end())
				{
					vector<string> temp;
					temp.push_back(temps);
					map[pattern] = temp;
					//cout << temps << endl;
				}
				else
				{
					vector<string>temp(map.at(pattern));
					temp.push_back(temps);
					map[pattern] = temp;
					//cout << temps << endl;
				}
			}

			//tree->~RBTreeMap();
			cout << "zzz" << endl;
			tree = new RBTreeMap;

			//cout << "zzz" << endl;
			//print all the possible pattern and store in a tree
			for (std::pair<std::string, vector<string>> element : map)
			{
				cout << element.first << " :: " << element.second.size() << endl;
				tree->add(element.first, element.second);
			}
			//map.clear();
			//cout << "--------------------" << endl;
			//tree->printTree();
			//cout << "rrr" << endl;
			Node *qt = tree->getMax();
			//choose the greatest occured pattern from a tree
			for (int i = 0; i < qt->key.length(); i++)
			{
				if (qt->key[i] == c[0])
				{
					answer[i] = c[0];
					count++;
				}
			}

			guess--;
			cout << "Updated answer: " << answer << endl;
			cout << "Guesses left: " << guess << endl;
			cout << "Max possibility: " << qt->length << endl;

		}

		if (guess >= 0 && count == length)
		{
			cout << "You got the word :  " << answer << endl;
		}
		else if (guess == 0 && count != length)
		{
			cout << "You lost all your chance!!" << endl;
		}
	}


	return 0;

}

