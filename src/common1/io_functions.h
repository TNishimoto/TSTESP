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
//#include "zstring.h"
using namespace std;
namespace my{
    class IO
	{
	  public:
		static unique_ptr<string> loadString(string &filename);
		static bool load(ifstream& stream, string& output);

		template <typename T>
		static unique_ptr<vector<T>> loadVector(string &filename)
		{
			std::ifstream file;
			file.open(filename, std::ios::binary);

			if (!file)
			{
				std::cerr << "error reading file: " << filename.c_str() << endl;
				exit(EXIT_FAILURE);
			}
			file.seekg(0, ios::end);
			auto n = (unsigned long)file.tellg();
			file.seekg(0, ios::beg);

			auto input = new vector<T>(); //データの値を格納する配列
			input->resize(n / sizeof(T));

			file.read((char *)&(*input)[0], n);
			file.close();
			file.clear();

			return make_unique<vector<T>>(*input);
		}

		/*
		template <typename T>
		static unique_ptr<zstring<T>> loadZString(string &filename)
		{
			auto p = loadVector<T>(filename);
			return make_unique<zstring<T>>(*p);
		}
		*/

		
		template <typename T>
		static unique_ptr<vector<T>> loadVector(std::ifstream& file)
		{
		
			file.seekg(0, ios::end);
			auto n = (unsigned long)file.tellg();
			file.seekg(0, ios::beg);

			auto input = new vector<T>(); //データの値を格納する配列
			input->resize(n / sizeof(T));

			file.read((char *)&(*input)[0], n);
			file.close();
			file.clear();

			return make_unique<vector<T>>(*input);
		}
		template <typename T>
		static void loadVec(std::ifstream& file, vector<T> &vec)
		{

			file.seekg(0, ios::end);
			auto n = (unsigned long)file.tellg();
			file.seekg(0, ios::beg);

			vec.resize(n / sizeof(T));

			file.read((char *)&(vec)[0], n);
			file.close();
			file.clear();
		}
		
		template <typename T>
		static bool writeVector(ofstream &out, vector<T> &text)
		{
			int count = text.size();
			out.write(reinterpret_cast<const char *>(&text[0]), text.size() * sizeof(T));
			out.close();
			return true;
		}
		template <typename T>
		static bool writeVector(string &filename, vector<T> &text)
		{
			ofstream out(filename, ios::out | ios::binary);
			if (!out)
				return 1;
			return writeVector<T>(out, text);
		}
		static unique_ptr<vector<int>> loadIntegers(string &filename);
		static bool write(string &filename, vector<int32_t> &text);
		static bool write(ofstream& os, std::string& text) {

			os.write((const char *)(&text[0]), sizeof(char) * text.size());
			return true;
		}
	};
}