/*
* Copyright (c) 2018 Takaaki Nishimoto
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published bytes
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "search_query.hpp"
#include <cstdlib>
#include "cmdline.h"
#include "tstesp.h"
#include "string_functions.h"

using namespace std;

void locate(ESPIndex &esp, tst::TST &tst, SearchQuery &st, string &patternFile)
{
  string line;
  ifstream pfs;
  pfs.open(patternFile, std::ios::binary);
  uint64_t time = 0, occ = 0;
  auto c = 0;
  auto n = 0;
  vector<uint64_t> result;
  std::chrono::system_clock::time_point start, end;
  while (getline(pfs, line))
  {
    if (line[line.size() - 1] == '\r')
      line.erase(line.size() - 1);
    std::cout << "query : " << ++n << " / " << line << std::endl;
    //std::cout << line.size() << std::endl;

    start = std::chrono::system_clock::now();
    //auto pattern = tst::istring(line);
    tst::istring pattern(line.begin(), line.end());
    tstesp::TSTESPIndex::locate(esp, tst, st, pattern, result);
    end = std::chrono::system_clock::now();

    time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    occ += result.size();
    c += line.size();

    if (result.size() < 500)
    {
      std::cout << "all occurrences : " << my::MyStrings::toIntegerString(result) << std::endl;
    }
    else
    {
      std::cout << "all occurrences : omit " << std::endl;
    }
    cout << "Time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << endl;
  }
  tstesp::showResult(time, occ, c, n);

  pfs.close();
}

void count(ESPIndex &esp, tst::TST &tst, SearchQuery &st, string &patternFile)
{
  string line;
  ifstream pfs;
  pfs.open(patternFile, std::ios::binary);
  uint64_t time = 0, occ = 0;
  auto c = 0;
  auto n = 0;
  std::chrono::system_clock::time_point start, end; // 型㝯 auto 㝧坯

  while (getline(pfs, line))
  {
    if (line[line.size() - 1] == '\r')
      line.erase(line.size() - 1);
    std::cout << "query : " << ++n << " / " << line << std::endl;
    start = std::chrono::system_clock::now();
    //auto pattern = tst::istring(line);
    tst::istring pattern(line.begin(), line.end());
    auto _occ = tstesp::TSTESPIndex::count(esp, tst, st, pattern);
    end = std::chrono::system_clock::now();
    time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    occ += _occ;
    c += line.size();
    std::cout << "OCC : " << _occ << ", Time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << endl;
  }
  tstesp::showResult(time, occ, c, n);

  pfs.close();
}

int main(int argc, char **argv)
{
  cmdline::parser p;
  p.add<string>("index_file", 'i', "index file name", true);
  p.add<string>("query_file", 'q', "query file name", true);
  p.add<string>("mode", 'm', "mode \"count\" or \"locate\"", true);
  p.parse_check(argc, argv);

  string inputFile = p.get<string>("index_file");
  string pattern_file = p.get<string>("query_file");
  string mode = p.get<string>("mode");
  string esp_index_file = inputFile + ".tesp";
  string tst_index_file = inputFile + ".tst";

  [[maybe_unused]] tstesp::TSTESPIndex index;

  ifstream tstStream;
  tstStream.open(tst_index_file, ios::binary);
  if (!tstStream)
  {
    std::cout << "No TST Index!" << std::endl;
    return 1;
  }

  ifstream espStream;
  espStream.open(esp_index_file, ios::binary);
  if (!espStream)
  {
    std::cout << "No ESP Index!" << std::endl;
    return 1;
  }

  //index.load(tstStream, espStream);
  ESPIndex esp_index;
  tst::TST tst;
  std::cout << "loading TST index..." << std::endl;
  tst.load(tstStream);
  std::cout << "loaded" << std::endl;
  std::cout << "loading ESP index..." << std::endl;
  esp_index.Load(espStream);
  std::cout << "loaded." << std::endl;
  tstStream.close();
  espStream.close();

  SearchQuery st;
  st.esp_index_ = &esp_index;

  //SearchQuery st;
  if (mode == "count")
  {
    count(esp_index, tst, st, pattern_file);
  }
  else if (mode == "locate")
  {
    locate(esp_index, tst, st, pattern_file);
    //index.locate(pattern_file);
    //st.LocateQuery(ifs,pfs);
  }
  else
  {

    cerr << "error" << endl;
    exit(1);
  }

  return 0;
}
