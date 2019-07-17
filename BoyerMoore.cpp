// Boyer-Moore
#include <iostream>
#include <map>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using chrono::duration_cast;
using chrono::milliseconds;

typedef chrono::steady_clock the_clock;

map<char, int> BadMatchTable(string pattern) // for bad character rule
{
	int patternLen = pattern.length;
	int p = patternLen - 1;
	int s = patternLen - 1;
	int x = 0;
	int skips = 0;
	char patternX = pattern.at(x); // X character in pattern
	map<char, int> patternMap;


	while (x < patternLen) // because its 0 indexed if x was = patternLen it would call a non-existant character
	{
		skips = patternLen - x - 1;

		if (patternMap.find(patternX) == patternMap.end()) // if character in pattern skip according to skip formula (above) else skip whole pattern
		{
			patternMap[patternX] = skips;
		}
		else
		{
			patternMap.insert(pair<char, int>(patternX, skips));
		}

		x++;

	}
	return patternMap;
}

vector<int> BoyerMoore(string pattern, string stringToSearch)
{
	int patternLen = pattern.length;
	int p = patternLen - 1;
	int s = patternLen - 1;
	char patPoint = pattern.at(p);
	char strPoint = stringToSearch.at(s);
	int currentLoc = 0;
	vector<char> suffix;
	int x = 0; // 
	vector<int> matches;
	map <char, int> skipTable = BadMatchTable(pattern);

	while (s < stringToSearch.length)
	{
		//// BAD CHARACTER RULE ////

		if (patPoint == strPoint) // if current letter in pattern is equal to current letter in string
		{
			suffix.push_back(patPoint); // add to suffix table
			p -= 1; // move left
			s -= 1;
			currentLoc -= 1;
		}
		else if (skipTable.find(strPoint) != skipTable.end()) // if not a letter in the pattern move whole pattern
		{
			p = patternLen - 1;
			s += patternLen - 1;
		}
		else // else move according to skip table
		{
			s += skipTable[patPoint];

		}

		if (p == 0 && patPoint == strPoint) // if at the end of the pattern and a match then add the location to match vector
		{
			matches.push_back(s);
		}
		else
		{
			//// GOOD SUFFIX RULE ////

			while (x < pattern.length - currentLoc) // loop through the rest of the pattern thats not been looked at yet
			{
				patPoint = pattern.at(x);
				int y = 0;

				if (patPoint == suffix.at(y)) // if letters in the suffix exist in the rest of the pattern then keep searching
				{
					y++;
					x++;
				}
				else // else just go back to start of suffix and look further on
				{
					x++;
					y = 0;
				}

				if (patPoint == suffix.at(suffix.size()) && patPoint == strPoint) // if whole suffix is found and it matches the current letter in the string then search the prefix to check for matches
				{
					s -= 1;
					p -= 1;
				}
				else // or just go back to the start
				{
					p = pattern.length - 1;
					s += pattern.length - 1;
				}

			}
			suffix.clear(); // at the end clear the suffix vector as not needed anymore
		}
	}
	return matches; // return location of matches
}


int main()
{
	string pattern;
	ifstream textFile("FlexTape.txt");
	string stringToSearch((istreambuf_iterator<char>(textFile)), (istreambuf_iterator<char>()));
	textFile.close();

	cout << "Please enter the pattern that you are searching for ";
	cin >> pattern; 

	vector<int> matchFound = BoyerMoore(pattern, stringToSearch);
	int x = 0;
	the_clock::time_point start = the_clock::now();

	while (x <= matchFound.size()) // go through matches vector and print the locations for the user
	{
		cout << "Match found at " << matchFound[x];
	}
	ofstream csvFile;
	csvFile.open("BMresults.csv");

	the_clock::time_point end = the_clock::now();
	auto miliSecs = duration_cast<milliseconds>(end - start).count();

	csvFile << miliSecs;
	csvFile.close();

	return 0;
}