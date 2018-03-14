#include "io_functions.h"
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
unique_ptr<string> IO::loadString(string &filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary);

	if (!file)
	{
		std::cerr << "error reading file: " << filename.c_str() << endl;
		string s = "";
		std::cin >> s;
		exit(EXIT_FAILURE);
	}
	file.seekg(0, ios::end);
	auto n = (unsigned long)file.tellg();
	file.seekg(0, ios::beg);

	auto input = new string(); //データの値を格納する配列
	input->resize(n / sizeof(char));

	file.read((char *)&(*input)[0], n);
	file.close();
	file.clear();

	return unique_ptr<string>(input);
}
bool IO::load(std::ifstream &file, string& output)
{

	if (!file)
	{
		std::cerr << "error reading file " << endl;
		return false;
	}
	file.seekg(0, ios::end);
	auto n = (unsigned long)file.tellg();
	file.seekg(0, ios::beg);

	output.resize(n / sizeof(char));

	file.read((char *)&(output)[0], n);
	file.close();
	file.clear();

	return true;
}


unique_ptr<vector<int32_t>> IO::loadIntegers(string &filename)
{
	return IO::loadVector<int32_t>(filename);
}
bool IO::write(string &filename, vector<int32_t> &text)
{
	IO::writeVector<int32_t>(filename, text);
	return true;
}
}