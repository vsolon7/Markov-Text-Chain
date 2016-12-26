// Markov Text Chain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <random>
#include <ctime>
#include "Ngram.h"

std::string txt;

void getWordGramVector(std::string);
void getNGramVector(std::string, int);
void swap(std::vector<Ngram> &, int, int);
std::vector<std::string> splitWord(const char *);
void markovIt(std::vector<Ngram>, int);
void wordsMarkovIt(std::vector<Ngram> vec);

int main()
{
	std::ifstream newFile("MarkovText.txt");
	if (newFile.is_open())
	{
		std::string line;
		while (getline(newFile, line))
		{
			txt += line + " ";
		}
		newFile.close();
	}

	getWordGramVector(txt);

	std::cout << std::endl;

	//system("pause");

	//getNGramVector(txt, 3);

	//std::cout << std::endl;

	system("pause");
    return 0;
}

//gets the amount of times every word appears in the text
void getWordGramVector(std::string txt)
{
	std::vector<Ngram> ngramVec;
	//splits it to only words.
	std::vector<std::string> words = splitWord(txt.c_str());

	//adds a new word to the list, if it already exists, increases the count of the word
	for (unsigned int i = 0; i < words.size() - 1; i++)
	{
		static double percentDone = 0;
		int index = -1;
		for (unsigned int j = 0; j < ngramVec.size(); j++)
		{
			if (words[i] == ngramVec[j].text)
				index = j;
		}
		if (index == -1)
		{
			Ngram n(words[i]);
			n.strAfter.push_back(words[i + 1]);
			ngramVec.push_back(n);
		} else
		{
			ngramVec[index].strAfter.push_back(words[i + 1]);
			ngramVec[index].count++;
		}
		std::cout.precision(5);
		percentDone = (100) * ((double)i / words.size());

		if (i % 20 == 0)
			std::cout << std::fixed << percentDone << "% done \n";
	}
	std::cout << "100% done \n-------------------------------------------\n";
	system("cls");

	int cin;
	std::cout << "How many paragraphs to generate?: ";
	do
	{
		std::cin >> cin;
	} while (cin < 0);

	for (int x = 0; x < cin; x++)
		wordsMarkovIt(ngramVec);

}

//makes a vector of ngrams (a substring n characters long), each with an array that holds a possible next character value
void getNGramVector(std::string txt, int order)
{
	std::vector<Ngram> ngramVec;
	//adds a new word to the list, if it already exists, increases the count of the word
	for (unsigned int i = 0; i < txt.size() - (order + 1); i++)
	{
		static double percentDone = 0;
		int index = -1;
		std::string gram = txt.substr(i, order);

		//does it already exist?
		for (unsigned int j = 0; j < ngramVec.size(); j++)
		{
			if (gram == ngramVec[j].text)
				index = j;
		}
		if (index == -1)
		{
			Ngram n(gram);
			n.after.push_back(txt[i + order]);
			ngramVec.push_back(n);
		} else
		{
			ngramVec[index].after.push_back(txt[i + order]);
			ngramVec[index].count++;
		}

		std::cout.precision(5);
		percentDone = (100) * ((double)i / (txt.size() - (order + 1)));

		if (i % 20 == 0)
			std::cout << std::fixed << percentDone << "% done \n";
	}
	std::cout << "100% done \n-------------------------------------------\n";
	system("cls");

	int cin;
	std::cout << "How many paragraphs to generate?: ";
	do
	{
		std::cin >> cin;
	} while (cin < 0);

	for(int x = 0; x < cin; x++)
		markovIt(ngramVec, order);
}

//swaps vec[x] and vec[y]
void swap(std::vector<Ngram> &vec, int x, int y)
{
	Ngram temp = vec[x];
	vec[x] = vec[y];
	vec[y] = temp;
}

//creates a vector where each element is a word.
std::vector<std::string> splitWord(const char *str)
{
	std::vector<std::string> result;

	do
	{
		const char *begin = str;

		while (*str != ' ' && *str != ';' && *str != ',' && *str != '.' && *str != '-' && *str != '"' && *str != '!' && *str != '~' && *str)
			str++;

		result.push_back(std::string(begin, str));
	} while (0 != *str++);

	//removes the null terminating characters in the array.
	for (unsigned int i = 0; i < result.size(); i++)
	{
		while (i < result.size() && result[i] == "")
		{
			if (result[i] == "")
				result.erase(result.begin() + i);
		}
	}

	return result;
}

//markov's a selection of text with ngrams
void markovIt(std::vector<Ngram> vec, int order)
{
	std::mt19937 randEngine(time(0));
	std::string currentGram = vec[0].text;

	std::string result = currentGram;
	unsigned int characters = 2000;

	//adds a random character, that in the source text, appears after an ngram. Then the last order with the new character is the new ngram
	//repeat however long characters is.
	for (unsigned int i = 0; i < characters; i++)
	{
		static int index = 0;
		static double percentDone = 0;

		std::vector<char> possibilities = vec[index].after;
		std::uniform_int_distribution<int> generator(0, possibilities.size() - 1);

		int next = generator(randEngine);

		result += vec[index].after[next];
		currentGram = result.substr(result.size() - order, result.size());

		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].text == currentGram)
				index = i;
		}
		percentDone = (100) * ((double)i / characters);

		if(i % 20 == 0)
			std::cout << std::fixed << percentDone << "% done \n";
	}
	std::cout << "100% done\n------------------------------------------\n";
	std::cout << result;
}

//markov's a number of words with a words
void wordsMarkovIt(std::vector<Ngram> vec)
{
	std::mt19937 randEngine(time(0));
	std::string currentGram = vec[0].text;

	std::string result = currentGram;
	unsigned int words = 75;

	//picks a random word that, in the source text, appears after the word before it.
	for (unsigned int i = 0; i < words; i++)
	{
		static int index = 0;
		static double percentDone = 0;

		std::vector<std::string> possibilities = vec[index].strAfter;
		std::uniform_int_distribution<int> generator(0, possibilities.size() - 1);

		int next = generator(randEngine);

		result += " " + vec[index].strAfter[next];
		currentGram = vec[index].strAfter[next];

		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].text == currentGram)
				index = i;
		}

		percentDone = (100) * ((double)i / words);

		if (i % 20 == 0)
			std::cout << std::fixed << percentDone << "% done \n";
	}
	std::cout << "100% done\n------------------------------------------\n";
	std::cout << result << "\n------------------------------------------\n";
}