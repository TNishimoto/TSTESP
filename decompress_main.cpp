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

#include<cstdlib>

#include "esp_index.hpp"
#include "cmdline.h"
#include "get_time.hpp"
#include "tstesp.h"
using namespace std;

int main(int argc,char** argv){

  cmdline::parser p;
  p.add<string>("input_file",  'i', "index file name",  true);
  p.add<string>("output_file", 'o', "output file name", true);
  p.parse_check(argc, argv);
  
  string inputFile   = p.get<string>("input_file");
  string output_file  = p.get<string>("output_file");
  string esp_index_file = inputFile + ".tesp";
  string tst_index_file = inputFile + ".tst";



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

  vector<uint8_t> outputText;
  tstesp::TSTESPIndex::decompress(esp_index, tst, "decompress_tmpfile", outputText);
  //string outputText2(outputText.begin(), outputText.end());
  
  //std::cout << outputText2 << std::endl;
  ofstream os;
  os.open(output_file);
	os.write((const char *)(&outputText[0]), sizeof(char) * outputText.size());
  os.close();

/*
  tstesp::TSTESPIndex index;
  auto b = index.load(inputFile);
  if(!b) return 1;
  ofstream outputStream;
  outputStream.open(output_file);
  index.decompress(outputStream);
*/

}
