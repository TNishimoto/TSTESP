#include "string_functions.h"
#include <chrono>
#include <time.h>
#include <string>
#include <bitset>
//#include <stdio.h>
//#include <windows.h>
//#include <Psapi.h>      // Psapi.Lib
using namespace std;
using namespace chrono;

namespace my
{
unique_ptr<string> MyStrings::createRandomString(int len, int alphabetSize)
{
	vector<char> alphabets;
	alphabets.push_back('a');
	alphabets.push_back('b');
	alphabets.push_back('c');
	alphabets.push_back('d');
	alphabets.push_back('e');

	std::random_device rnd;										  // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());										  //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, alphabetSize - 1); // [0, 99] 範囲の一様乱数

	auto r = new string();
	for (int i = 0; i < len; i++)
	{
		r->append(1, alphabets[rand100(mt)]);
	}
	return unique_ptr<string>(r);
}

unique_ptr<string> MyStrings::createDeterministicRandomString(int len, int alphabetSize, int seed)
{
	vector<char> alphabets;
	alphabets.push_back('a');
	alphabets.push_back('b');
	alphabets.push_back('c');
	alphabets.push_back('d');
	alphabets.push_back('e');

	while(alphabets.size() > alphabetSize) alphabets.pop_back();
	
	return MyStrings::createDeterministicRandomString(len,alphabets,seed);
}
unique_ptr<string> MyStrings::createDeterministicRandomString(int len, vector<char> &alphabets, int seed)
{
	std::mt19937 mt(seed);										  //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, alphabets.size() - 1); // [0, 99] 範囲の一様乱数

	auto r = new string();
	for (int i = 0; i < len; i++)
	{
		r->append(1, alphabets[rand100(mt)]);
	}
	return unique_ptr<string>(r);
}
unique_ptr<vector<char>> MyStrings::GetAlphabets(string &text)
{
	auto occur = vector<bool>();
	occur.resize(256);
	for (int i = 0; i < 256; i++)
		occur[i] = false;
	
	for (auto it : text)
	{
		occur[(unsigned char)it] = true;
	}
	
	auto r = make_unique<vector<char>>();
	for (int i = 0; i < 256; i++)
	{
		if (occur[i])
			r->push_back((char)i);
	}
	return r;
}

std::pair<int, int> MyStrings::createRandomIntegerPair(int seed, int max)
{
	std::mt19937 mt(seed);								 //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, max - 1); // [0, 99] 範囲の一様乱数

	int a = rand100(mt);
	int b = rand100(mt);
	if (a < b)
	{
		return std::pair<int, int>(a, b);
	}
	else
	{
		return std::pair<int, int>(b, a);
	}
}

unique_ptr<vector<int>> MyStrings::createRandomInteger(int len, int max)
{

	std::random_device rnd;								 // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());								 //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, max - 1); // [0, 99] 範囲の一様乱数

	auto p = new vector<int>();

	for (int i = 0; i < len; i++)
	{
		auto value = rand100(mt);
		p->push_back(value);
	}
	return unique_ptr<vector<int>>(p);
}
int MyStrings::createRandomInt(int max, int seed)
{

	std::mt19937 mt(seed);								 //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, max - 1); // [0, 99] 範囲の一様乱数
	auto value = rand100(mt);
	return value;
}
unique_ptr<vector<int>> MyStrings::createDeterministicRandomIntegers(int len, int max, int seed)
{
	//std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(seed);								 //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, max - 1); // [0, max] 範囲の一様乱数

	auto p = new vector<int>();

	for (int i = 0; i < len; i++)
	{
		auto value = rand100(mt);
		p->push_back(value);
	}
	return unique_ptr<vector<int>>(p);
}
unique_ptr<string> MyStrings::createFibonacciWord(int length)
{
	string p0 = "a";
	string p1 = "b";
	string p2 = p1 + p0;
	int t = 1;
	while ((int)p2.size() < length)
	{
		p2 = p1 + p0;
		t++;
		p0 = p1;
		p1 = p2;
		//std::cout << p2.size() << std::endl;
	}
	return unique_ptr<string>(new string(p2));
}
}