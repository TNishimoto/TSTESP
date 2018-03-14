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

#include <cstdlib>
#include "extract.hpp"
#include "cmdline.h"
#include "tstesp.h"

using namespace std;

void extract(ESPIndex &esp, tst::TST &tst, Extract &ext, ifstream &pfs)
    {
        //auto r = make_unique<vector<unique_ptr<istring>>>();
        uint64_t start_pos, end_pos;
        string line;
        stringstream ss;
        tst::istring output;
        while(getline(pfs,line)){
            ss << line;
            ss >> start_pos;
            ss >> end_pos;
            ss.clear();
            line.clear();

            output.clear();
            std::cout << "Query : " << start_pos << "/" << end_pos << std::endl;

            tstesp::TSTESPIndex::extract(esp, tst, ext, start_pos, end_pos - start_pos + 1, output);
            string output2(output.begin(), output.end());
            std::cout << output2 << std::endl;
            //r->push_back(this->extract(start_pos, end_pos - start_pos + 1));
        }
    }
void extract(ESPIndex &esp, tst::TST &tst, Extract &ext){
        tst::istring output;
        tstesp::TSTESPIndex::extract(esp, tst, ext, 0, 2, output);
        string output2(output.begin(), output.end());
            std::cout << output2 << std::endl;
}
int main(int argc, char** argv){
  
  cmdline::parser p;
  
  p.add<string>("index_file", 'i', "index file name",true);
  p.add<string>("position_file", 'p', "position file name",true);
  p.parse_check(argc,argv);
  
  
  string inputFile = p.get<string>("index_file");
  
  string esp_index_file = inputFile + ".tesp";
  string tst_index_file = inputFile + ".tst";
  string position_file = p.get<string>("position_file");
  
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

  ifstream positionStream;
  positionStream.open(position_file, ios::binary);
  if (!positionStream)
  {
    std::cout << "No Position File!" << std::endl;
    return 1;
  }


  Extract ext;
  ext.esp_index_ = &esp_index;
  extract(esp_index, tst, ext, positionStream);
  extract(esp_index, tst, ext);

  //vector<uint8_t> outputText;
  //tstesp::TSTESPIndex::decompress(esp_index, tst, "decompress_tmpfile", outputText);
  //string outputText2(outputText.begin(), outputText.end());
  
  //std::cout << outputText2 << std::endl;
  //ofstream os;
  //os.open(output_file);
	//os.write((const char *)(&outputText[0]), sizeof(char) * outputText.size());
  //os.close();
  
  /*
  ifstream pfs(position_file.c_str());
  
  auto index = make_unique<tstesp::TSTESPIndex>();
  index->load(tst_index_file, esp_index_file);
  auto result = index->extract(pfs);

  //ifstream ifs(input_file.c_str());
  
  for(auto& it : *result){
    std::cout << *it->toString() << std::endl;
  }
  */
  return 0;
}
