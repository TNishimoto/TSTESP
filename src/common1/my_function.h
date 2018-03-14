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

//#define DISP_PARAM(n)    (n), ((n) / 1024)
class MyFunction
{
  public:
	

  private:
	static int debugCounter;
	static string logName;

  public:
	
	static int lcp(std::string &item1, std::string &item2);
	static unsigned char *getFileContent(string filename, unsigned long &n, int termin);
	static bool createSampleFiles();
	static bool readAndSave(char *filename, int length, char *outputName);
	static bool write(string filename, std::string &text);
	static bool write(string filename, std::string &text, int length);
	static bool write(string filename, vector<int> &items);
	static bool write(string filename, vector<std::string> &text);
	static bool debugWrite(std::string &text);
	static bool debugWrite(std::string &text, string &name);
	static bool debugAppend(std::string &text);

	static unique_ptr<std::string> readFile(string filename);
	static unique_ptr<string> reverse(string &str);
	static int longestCommonSuffix(string &left, string &right);
	static int longestCommonPrefix(string &left, string &right);

	//template <class X>
	//static void print(vector<X> &items);
	static int max(vector<int> &items);

	static unique_ptr<string> joinLineString(vector<std::string> &items);
};
class PointerDeletedException : public std::exception
{
};

template <typename T>
unique_ptr<T[]> toArray(vector<T> &item)
{
	auto r = make_unique<T[]>(item.size());
	for (int i = 0; i < item.size(); i++)
	{
		r[i] = item[i];
	}
	return r;
}

template <typename T>
T sum(vector<T> &item)
{
	T r = 0;
	for(auto it : item){
		r += it;
	}
	return r;
}
template <typename T>
bool equal(vector<T> &item1, vector<T> &item2)
{
	if(item1.size() != item2.size()) return false;
	for(int i=0;i<item1.size();i++){
		if(item1[i] != item2[i]){
			return false;
		}
	}
	return true;
}

}