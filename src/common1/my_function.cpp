#include "my_function.h"
#include <chrono>
#include <time.h>
#include <string>
#include <bitset>
//#include <stdio.h>
//#include <windows.h>
//#include <Psapi.h>      // Psapi.Lib
using namespace std;
using namespace chrono;

namespace my {



int MyFunction::debugCounter = 0;
string MyFunction::logName = string();


		 int MyFunction::lcp(std::string& item1, std::string& item2) {
			auto min = std::min(item1.size(), item2.size());
			int k = 0;
			for (unsigned int i = 0; i < min; i++) {
				if (item1[i] == item2[i]) {
					k++;
				}
				else {
					break;
				}
			}
			return k;
		}
		 unsigned char *MyFunction::getFileContent(string filename, unsigned long &n, int termin) {
			std::ifstream file(filename);
			if (!file)
			{
				cerr << "error reading file: " << filename.c_str() << endl;
				exit(EXIT_FAILURE);
			}

			file.seekg(0, ios::end);
			n = (unsigned long)file.tellg();
			if (termin)
				n++;
			file.seekg(0, ios::beg);

			unsigned char *text = new unsigned char[n + 1];

			char c;
			unsigned long i = 0;

			while (file.get(c))
			{
				text[i] = c;//(c=='\n'||c=='\r')?'X':c;
				i++;
			}
			file.close();

			if (termin)
				text[n - 1] = 0;
			text[n] = 0;
			return text;
		}
		 bool MyFunction::createSampleFiles() {
			string file1 = "../../data/dna.50MB";
			auto str = readFile(file1);
			auto len = (int)(*str).size();
			write(const_cast<char*>("../../data/dna.5MB"), *str, len / 10);
			write(const_cast<char*>("../../data/dna.05MB"), *str, len / 100);
			write(const_cast<char*>("../../data/dna.005MB"), *str, len / 1000);
			write(const_cast<char*>("../../data/dna.0005MB"), *str, len / 10000);
			return true;
		}
		 bool MyFunction::readAndSave(char* filename, int length, char* outputName) {
			auto str = readFile(filename);
			return write(filename, *str, length);

		}
		 bool MyFunction::write(string filename, std::string& text) {
			return write(filename, text, (int)text.size());
		}
		 bool MyFunction::write(string filename, std::string& text, int length) {
			std::ofstream ofs(filename);
			for (int i = 0; i < length; i++) {
				ofs << text[i];
			}
			ofs.close();
			return true;

		}
		 bool MyFunction::write(string filename, vector<int>& items) {
			std::ifstream file(filename);
			if (file) {
				std::cout << "File Exist Error : " << filename.c_str() << std::endl;
				return false;
			}
			else {
				ofstream ofs;
				ofs.open(filename, ios::out | ios::trunc);

				for (int i = 0; i < (int)items.size(); i++) {
					ofs << items[i] << std::endl;
				}
				ofs.close();
				return true;

			}
		}
		 bool MyFunction::write(string filename, vector<std::string>& text) {
			std::ofstream ofs(filename);

			for (auto it : text) {
				for (unsigned int i = 0; i < it.size(); i++) {
					ofs << it[i];
				}
				ofs << std::endl;
			}
			ofs.close();
			return true;
		}
		bool MyFunction::debugWrite(std::string& text){
			string p = "";
			return MyFunction::debugWrite(text, p);
		}
		bool MyFunction::debugWrite(std::string& text, string& name) {
			auto start = std::chrono::system_clock::now();
			time_t t = std::chrono::system_clock::to_time_t(start);

			MyFunction::write("log_" + std::to_string(time(&t)) + "_" + std::to_string(MyFunction::debugCounter++) + "_" + name + ".txt", text);
			return true;
		}
		bool MyFunction::debugAppend(std::string& text) {
			if (MyFunction::logName.size() == 0) {
				auto start = std::chrono::system_clock::now();
				time_t t = std::chrono::system_clock::to_time_t(start);
				MyFunction::logName = std::to_string(time(&t));
			}

			auto filename = string("log_") + MyFunction::logName + string(".txt");
			ofstream fout(filename, ios::app);
			fout << text << std::endl;
			fout.close();
			return true;
		}


		 unique_ptr<std::string> MyFunction::readFile(string filename) {
			std::ifstream file(filename);

			if (!file)
			{
				std::cerr << "error reading file: " << filename.c_str() << endl;
				exit(EXIT_FAILURE);
			}

			file.seekg(0, ios::end);
			auto n = (unsigned long)file.tellg();
			file.seekg(0, ios::beg);

			//char *text = new char[n + 1];

			char c;
			unsigned long i = 0;

			auto str = new string();
			//std::cout << "read text size = " << n << std::endl;
			while (file.get(c))
			{
				//text[i] = c;//(c=='\n'||c=='\r')?'X':c;
				//std::cout << c << std::flush;
				str->push_back(c);
				i++;
			}
			file.close();

			//text[n] = 0;
			//delete[] text;
			return unique_ptr<string>(str);
		}
		 unique_ptr<string> MyFunction::reverse(string& str) {
			string p = string();
			for (int i = (int)str.size() - 1; i >= 0; i--) {
				p = p + str[i];
			}
			return std::make_unique<string>(p);
		}
		 int MyFunction::longestCommonSuffix(string& left, string& right) {
			int min_len = std::min((int)left.size(), (int)right.size());
			int k = 0;
			while (k < min_len) {
				if (left[left.size() - k - 1] == right[right.size() - k - 1]) {
					k++;
				}
				else {
					break;
				}
			}
			return k;
		}
		 int MyFunction::longestCommonPrefix(string& left, string& right) {
			int min_len = std::min((int)left.size(), (int)right.size());
			int k = 0;
			while (k < min_len) {
				if (left[k] == right[k]) {
					k++;
				}
				else {
					break;
				}
			}
			return k;
		}

		
		 int MyFunction::max(vector<int>& items) {
			int k = 0;
			for (auto it : items) {
				if (k < it) k = it;
			}
			return 0;
		}

		unique_ptr<string> MyFunction::joinLineString(vector<std::string>& items){
			auto r = make_unique<string>();
			for(auto line : items){
				*r += line;
				*r += "\n";
			}
			return r;
		}
}