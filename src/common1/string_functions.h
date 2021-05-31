#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <exception>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <time.h>
//#include <stdio.h>
//#include <windows.h>
//#include <Psapi.h>      // Psapi.Lib
using namespace std;

namespace my
{

class MyStrings
{
  public:
	static unique_ptr<string> createRandomString(int len, int alphabetSize);

	static unique_ptr<string> createDeterministicRandomString(int len, int alphabetSize, int seed);

	static std::pair<int, int> createRandomIntegerPair(int seed, int max);

	static unique_ptr<vector<int>> createRandomInteger(int len, int max);
	static int createRandomInt(int max, int seed);
	static unique_ptr<vector<int>> createDeterministicRandomIntegers(int len, int max, int seed);
	static unique_ptr<string> createFibonacciWord(int length);
	static unique_ptr<string> createDeterministicRandomString(int len, vector<char>& alphabets, int seed);
	static unique_ptr<vector<char>> GetAlphabets(string& text);
	
	template <class X>
	static string toIntegerString(vector<X> &items)
	{
		string s = "";
		s += "[";
		[[maybe_unused]]int k = items.size();
		for (int i = 0; i < (int)items.size(); i++)
		{
			s += std::to_string(items[i]);
			if (i != (int)items.size() - 1)
				s += ", ";
		}
		s += "]";
		return s;
	}
	template <class X>
	static void toIntegerString(vector<X> &items, string& result)
	{
		result += "[";
		int k = items.size();
		for (unsigned int i = 0; i < (int)items.size(); i++)
		{
			result += std::to_string(items[i]);
			if (i != items.size() - 1)
				result += ", ";
		}
		result += "]";
	}
	template <class X>
	static void print(vector<X> &items)
	{
		std::cout << toIntegerString<X>(items) << std::endl;
	}
	template <typename T, typename U>
	static unique_ptr<vector<U>> toVector(vector<T> &item)
	{
		auto r = make_unique<vector<U>>();
		auto &r2 = *r;
		r->resize(item.size());
		for (int i = 0; i < item.size(); i++)
		{
			r2[i] = (U)item[i];
		}
		return r;
	}
	template <typename T>
	static bool checkEqual(vector<T> &left, vector<T> &right)
	{
		int min = std::min(left.size(), right.size());
		for (int i = 0; i<min; i++) {
			if (left[i] != right[i]) {
				return false;
			}
		}
		return (left.size() - right.size()) == 0;
	}
};
}