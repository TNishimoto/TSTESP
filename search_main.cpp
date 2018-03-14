/* 
 * Copyright (C) 2018, Takaaki Nishimoto, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#include "search_query.hpp"
#include <cstdlib>
#include "cmdline.h"
#include "tstesp.h"
#include "string_functions.h"

using namespace std;

void locate(ESPIndex &esp, tst::TST &tst, SearchQuery &st, string &patternFile){
        string line;
	    ifstream pfs;
        pfs.open(patternFile, std::ios::binary);
  uint64_t time = 0, occ = 0;
        auto c = 0;
        auto n = 0;
        vector<uint64_t> result;
        std::chrono::system_clock::time_point  start, end;
        while (getline(pfs, line)){
            if (line[line.size()-1] == '\r') line.erase(line.size()-1); 
            std::cout << "query : " << ++n  << " / " << line << std::endl;
            //std::cout << line.size() << std::endl;
            
            start = std::chrono::system_clock::now();
            //auto pattern = tst::istring(line);
            tst::istring pattern(line.begin(), line.end());
            tstesp::TSTESPIndex::locate(esp, tst, st, pattern, result);
            end = std::chrono::system_clock::now();  
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
            occ += result.size();
            c += line.size();

            if(result.size() < 500){
            std::cout << "all occurrences : " << my::MyStrings::toIntegerString(result) << std::endl;
            }else{
                std::cout << "all occurrences : omit " << std::endl;
            }            
			cout << "Time : " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << "ns" << endl;
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

  tstesp::TSTESPIndex index;

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
