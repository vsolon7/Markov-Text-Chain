#pragma once

#include <string>

struct Ngram
{
	std::string text;
	std::vector<char> after;
	std::vector<std::string> strAfter;
	int count;

	Ngram(std::string t)
	{
		text = t;
		count = 1;
	}
};