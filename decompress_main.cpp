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
